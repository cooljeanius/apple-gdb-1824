# Copyright (C) 2013 Free Software Foundation, Inc.

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

# This file is part of the GDB testsuite.  It tests Python-based
# frame-filters.
import gdb
import itertools
from gdb.FrameDecorator import FrameDecorator


class Reverse_Function(FrameDecorator):
    def __init__(self, fobj):
        super().__init__(fobj)
        self.fobj = fobj

    def function(self):
        fname = str(self.fobj.function())
        if fname == None or fname == "":
            return None
        else:
            fname = fname[::-1]
        return fname


class Dummy(FrameDecorator):
    def __init__(self, fobj):
        super().__init__(fobj)
        self.fobj = fobj

    def function(self):
        return "Dummy function"

    def address(self):
        return 0x123

    def filename(self):
        return "Dummy filename"

    def frame_args(self):
        return [("Foo", gdb.Value(12)), ("Bar", "Stuff"), ("FooBar", 42)]

    def frame_locals(self):
        return []

    def line(self):
        return 0

    def elided(self):
        return None


class FrameFilter:
    def __init__(self):
        self.name = "Reverse"
        self.priority = 100
        self.enabled = True
        gdb.frame_filters[self.name] = self

    def filter(self, frame_iter):
        # Python 3.x moved the itertools.imap functionality to map(),
        # so check if it is available.
        if hasattr(itertools, "imap"):
            frame_iter = map(Reverse_Function, frame_iter)
        else:
            frame_iter = list(map(Reverse_Function, frame_iter))

        return frame_iter


class ElidingFrameDecorator(FrameDecorator):
    def __init__(self, frame, elided_frames):
        super().__init__(frame)
        self.elided_frames = elided_frames

    def elided(self):
        return iter(self.elided_frames)


class ElidingIterator:
    def __init__(self, ii):
        self.input_iterator = ii

    def __iter__(self):
        return self

    def __next__(self):
        frame = next(self.input_iterator)
        if str(frame.function()) != "func1":
            return frame

        # Suppose we want to return the 'func1' frame but elide the
        # next frame.  E.g., if call in our interpreter language takes
        # two C frames to implement, and the first one we see is the
        # "sentinel".
        elided = next(self.input_iterator)
        return ElidingFrameDecorator(frame, [elided])

    # Python 3.x requires __next__(self) while Python 2.x requires
    # next(self).  Define next(self), and for Python 3.x create this
    # wrapper.
    def __next__(self):
        return next(self)


class FrameElider:
    def __init__(self):
        self.name = "Elider"
        self.priority = 900
        self.enabled = True
        gdb.frame_filters[self.name] = self

    def filter(self, frame_iter):
        return ElidingIterator(frame_iter)


FrameFilter()
FrameElider()
