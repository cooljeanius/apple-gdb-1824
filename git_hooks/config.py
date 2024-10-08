from errors import InvalidUpdate
from git import git, CalledProcessError
from type_conversions import to_type

import os
from tempfile import mkstemp
import sys

# A dictionary of all git config names that this module can query.
#   - The key used for this table is the config name.
#   - The value is another dictionary containing the following keys.
#       + 'default': The default value for this config option.

GIT_CONFIG_OPTS = {
    "hooks.allow-delete-tag": {"default": False, "type": bool},
    "hooks.allow-non-fast-forward": {"default": "", "type": tuple},
    "hooks.allow-lightweight-tag": {"default": False, "type": bool},
    "hooks.combined-style-checking": {"default": False, "type": bool},
    "hooks.commit-url": {"default": None},
    "hooks.debug-level": {"default": 0, "type": int},
    "hooks.disable-merge-commit-checks": {"default": False, "type": bool},
    "hooks.file-commit-cmd": {"default": None},
    "hooks.from-domain": {"default": None},
    "hooks.mailinglist": {"default": None, "type": tuple},
    "hooks.max-commit-emails": {"default": 100, "type": int},
    "hooks.max-email-diff-size": {"default": 100000, "type": int},
    "hooks.max-rh-line-length": {"default": 76, "type": int},
    "hooks.no-emails": {"default": "", "type": tuple},
    "hooks.no-precommit-check": {"default": "", "type": tuple},
    "hooks.no-rh-style-checks": {"default": "", "type": tuple},
    "hooks.post-receive-hook": {"default": None},
    "hooks.reject-merge-commits": {"default": "", "type": tuple},
    "hooks.style-checker": {"default": "cvs_check"},
    "hooks.tn-required": {"default": False, "type": bool},
    # The following options are for testing purposes only, and should
    # never be used in an operational repository.
    "hooks.bcc-file-ci": {"default": True, "type": bool},
}

# The maximum number of characters from a commit's subject
# to be used as part of the subject of emails describing
# the commit.
SUBJECT_MAX_SUBJECT_CHARS = 100


class UnsupportedOptionName(Exception):
    """An exception raised when trying to lookup an unsupported option name."""

    pass


# A map containing the value of the various git config options.
# Lazy initialized.
#
# It includes all options listed in GIT_CONFIG_OPTS; if the project
# does not set a given option, then the default value is used in
# its place.
__git_config_map = None

NO_REFS_META_CONFIG_WARNING = """\
-----------------------------------------------------------------
Unable to find file project.config from branch refs/meta/config
Using your repository's config file instead.

This is not a fatal issue, but please contact your repository's
administrator to set your project.config file up.
-----------------------------------------------------------------\
"""


def git_config(option_name):
    """Return the git config value for option_name.

    PARAMETERS
        option_name: The name of the git config option to query.
    """
    global __git_config_map

    if option_name not in GIT_CONFIG_OPTS:
        raise UnsupportedOptionName(option_name)

    if __git_config_map is None:
        initialize_git_config_map()
    val = __git_config_map[option_name]

    # If this option as a 'type' specified, then convert it to
    # this type if necessary.  We do this here, rather than during
    # initialize_git_config_map to avoid the potential for causing
    # an error for options which might not be used in the end.
    if "type" in GIT_CONFIG_OPTS[option_name] and isinstance(val, str):
        try:
            val = to_type(val, GIT_CONFIG_OPTS[option_name]["type"])
        except ValueError:
            TYPE_NAME_MAP = {
                bool: "boolean",
                int: "integer",
                tuple: "list",
            }
            type_name = TYPE_NAME_MAP[GIT_CONFIG_OPTS[option_name]["type"]]
            raise InvalidUpdate(
                "Invalid {} value: {} (must be {})".format(
                    option_name, val, type_name)
            )
        # Save the converted value to avoid having to do it again
        # the next time we query the same config option.
        __git_config_map[option_name] = val

    return val


def initialize_git_config_map():
    """Initialize the __git_config_map global."""
    global __git_config_map

    # The hooks' configuration is stored on a special branch called
    # refs/meta/config, inside a file called project.config.  Get
    # that file.
    (tmp_fd, tmp_file) = mkstemp("tmp-git-hooks-")
    try:
        cfg_file = tmp_file
        try:
            git.show("refs/meta/config:project.config", _outfile=tmp_fd)
        except CalledProcessError:
            # Most likely a project that still uses the repository's
            # config file to store the hooks configuration, rather
            # that the controlled project.config file.
            #
            # Handle this situation by doing what we used to do,
            # which is get the configuration from the repository's
            # config file, after having warned the user about it
            # (to expedite the transition).
            #
            # Note that we cannot use "utils.warn" to do the warning
            # in this module, as the "utils" module depends on this
            # module. Do the warning by hand.
            #
            # ??? One small issue is the fact that this warning may get
            # displayed multiple times (once per "phase", eg "update",
            # then "post-receive"). Given the relatively rare nature
            # of this event, we'll just accept it, instead of fancying
            # things up.
            for l in NO_REFS_META_CONFIG_WARNING.splitlines():
                print("*** %s" % l, file=sys.stderr)
            cfg_file = "config"
        os.close(tmp_fd)
        # Get the currently defined config values, all in one go.
        # Use "--file <cfg_file>" to make sure that we only parse
        # the file we just retrieved. Otherwise, git also parses
        # the user's config file.
        all_configs = git.config("-l", "--file", cfg_file, _split_lines=True)
    finally:
        os.unlink(tmp_file)

    all_configs_map = dict([config.split("=", 1) for config in all_configs])

    # Populate the __git_config_map dictionary...
    __git_config_map = {}
    for config_name in list(GIT_CONFIG_OPTS.keys()):
        # Get the config value from either the all_configs_map
        # if defined, or else from the default value.
        if config_name in all_configs_map:
            config_val = all_configs_map[config_name]
        else:
            config_val = GIT_CONFIG_OPTS[config_name]["default"]

        # Finally, save the config value if __git_config_map
        __git_config_map[config_name] = config_val
