# Copyright 2023 Free Software Foundation, Inc.

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import os

import gdb

from .server import request, capability
from .startup import send_gdb_with_response, in_gdb_thread


# The next available source reference ID.  Must be greater than 0.
_next_source = 1

# Map from full paths to Source dictionaries.
_source_map = {}

# Map from a source reference ID back to the same Source that is
# stored in _source_map.
_id_map = {}


@in_gdb_thread
def make_source(fullname, filename):
    """Return the Source for a given file name.

    FULLNAME is the full name.  This is used as the key.
    FILENAME is the base name.
    """
    global _source_map
    if fullname in _source_map:
        result = _source_map[fullname]
    else:
        result = {
            "name": filename,
            "path": fullname,
        }

        if not os.path.exists(fullname):
            global _next_source
            result["sourceReference"] = _next_source

            global _id_map
            _id_map[_next_source] = result
            _next_source += 1

        _source_map[fullname] = result
    return result


@in_gdb_thread
def decode_source(source):
    """Decode a Source object.

    Finds and returns the filename of a given Source object."""
    if "path" in source:
        return source["path"]
    if "sourceReference" not in source:
        raise Exception(
            "either 'path' or 'sourceReference' must appear in Source")
    ref = source["sourceReference"]
    global _id_map
    if ref not in _id_map:
        raise Exception("no sourceReference " + str(ref))
    return _id_map[ref]["path"]


@in_gdb_thread
def _sources():
    result = []
    for elt in gdb.execute_mi("-file-list-exec-source-files")["files"]:
        result.append(make_source(elt["fullname"], elt["file"]))
    return {
        "sources": result,
    }


@request("loadedSources")
@capability("supportsLoadedSourcesRequest")
def loaded_sources(**extra):
    return send_gdb_with_response(_sources)


# This helper is needed because we must only access the globals here
# from the gdb thread.
@in_gdb_thread
def _get_source(source):
    filename = decode_source(source)
    with open(filename) as f:
        content = f.read()
    return {
        "content": content,
    }


@request("source")
def source(*, source=None, sourceReference: int, **extra):
    # The 'sourceReference' parameter is required by the spec, but is
    # for backward compatibility, which I take to mean that the
    # 'source' is preferred.
    if source is None:
        source = {"sourceReference": sourceReference}
    return send_gdb_with_response(lambda: _get_source(source))
