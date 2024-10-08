**MacsBug Interface for GDB**
**8/21/06**

**1\. Introduction**

The MacsBug supported here is and extension to
[gdb](http://developer.apple.com/documentation/DeveloperTools/gdb/gdb/gdb_toc.html)
to support a subset of the Mac Classic
[MacsBug](http://developer.apple.com/tools/debuggers/MacsBug/) commands and a
MacsBug-like screen UI. Thus it basically is a variant of Classic MacsBug with
source-level debugging!

Along with this README there are three other files in this directory:

  * gdbinit-MacsBug-without-plugin
  * gdbinit-MacsBug
  * MacsBug_plugin

Both gdbinit-MacsBug-without-plugin and gdbinit-MacsBug are gdb command
language scripts one of which you **source** from your `~/.gdbinit` script (the
script that gdb always looks for, and for what it is worth, it looks for a
.gdbinit script in the current directory as well). In the following sections
the difference in the two MacsBug scripts will be explained.

The MacsBug_plugin is just that; the gdb plugins that implement the MacsBug UI
and commands. The plugin is loaded by the gdbinit-MacsBug script.



**2\. Background**

Once upon a time there was an implementation of Classic MacsBug for gdb using
nothing but the command language provided by gdb. The file was called gdbinit-MacsBug
(and since renamed gdbinit-MacsBug-without-plugin) and it is included
with this release. It supports about 40 of the MacsBug commands.

It has the benefit of being totally portable, i.e., it should be able to be
used with any version of gdb, on any Unix system, anywhere. Its down side is
that it cannot do any more that what is provided by the rather limited gdb
command language, is interpretive, and thus rather inefficient and in some
cases slow, and cannot provide a MacsBug-like screen UI.

To get around these limitations the plugin support in Apple's gdb was utilized
which allows the implementation be done in a "real" language. Thus the script
gdbinit-MacsBug is provided which appears to be a very reduced form or
gdbinit-MacsBug-without-plugin because most of the implementation is provided
in the plugin (MacsBug_plugin) which gdbinit-MacsBug loads with a **load-plugin**
Apple gdb command.

The benefit of using the plugin is that it is very efficient because it
"talks" directly to gdb. It can provide functionality that the pure script
variant cannot like some additional MacsBug commands and behavior as well as a
MacsBug-like screen UI. Its downside is of course that it is tied directly
to the version of gdb (and system) for which it was built. Further, because
the script uses the **load-plugin** gdb command it will only work with Apple's
version of gdb. Since MacsBug is mainly used by Mac developers this should be
not be a serious limitation.

You thus have two choices:

  1. Install the totally portable gdbinit-MacsBug-without-plugin script, or, 
  2. Install the gdbinit-MacsBug script and the MacsBug_plugin file. 



**3\. Installation**

The MacsBug files should already be installed in,

    /usr/libexec/gdb/plugins/MacsBug

If you elect choice 1 then just add the following to your `~/.gdbinit` script:

```gdb
  source /usr/libexec/gdb/plugins/MacsBug/gdbinit-MacsBug-without-plugin
```

If you want to use the plugin then add the following **source** command
instead:

```gdb
  source /usr/libexec/gdb/plugins/MacsBug/gdbinit-MacsBug
```

The gdbinit-MacsBug script is installed with its **load-plugin** command
defined as follows:

```gdb
  load-plugin /usr/libexec/gdb/plugins/MacsBug/MacsBug_plugin
```

The **load-plugin** command will load the MacsBug_plugin from its installed
directory when gdbinit-MacsBug is read (i.e., when the **source** command in
your `~/.gdbinit` script is executed).

Of course you don't have to use the files in their installed location. They
can be anywhere so long as the `~/.gdbinit`'s **source** command can access the
gdbinit-MacsBug file with an appropriate pathname and the gdbinit-MacsBug
script's **load-plugin** command can, in turn, access the MacsBug_plugin file.
The only restriction on the **load-plugin** command is that the specified
pathname must be a full pathname ('~' cannot be used).

From this point on this README assumes you are installing the plugin.



**4\. Using Gdb's HELP Commands For MacsBug**

Once gdb is loaded type **help** to see a summary of all gdb command classes.
There are three additional classes for MacsBug: "macsbug", "screen", and
"useful".

Typing **help macsbug** gives a summary of all the MacsBug commands.

Typing **help screen** gives a summary of all MacsBug screen UI-related
commands which are additions to this MacsBug implementation.

Typing **help useful** gives a list of "useful" gdb commands that could be
used to extend the MacsBug script or for your own gdb **define** commands.
Some of these are used internally, or are plugin descendants from the original
gdb script, or are just plain useful in their own right and should probably be
in gdb in the first place!.

Two existing gdb help classes, "aliases" and "support" also list some MacsBug
commands. The "aliases" class lists aliases for some commands that don't
follow the standard gdb command abbreviation conventions (e.g., **sc6** is the
same as **sc**). The "support" class lists the **set** options.

As indicated at the end of the **help** class summary you can type **mb-notes**
to get some additional information specifically related to this
implementation of MacsBug. These notes are also repeated in the next section.

Finally, also as indicated in the **help** summary, typing **help** followed
by a command name displays a summary for that specific command.



**5\. General Comments About Gdb MacsBug**

Below are a list of some general notes about the Gdb MacsBug. Much of this
information is also available by type "mb-notes" (or help mb-notes) into gdb.

  * The MacsBug support for gdb is as faithful a simulation as possible within the limits of the environment. There are also some extensions where it makes sense in the context of the debugging paradigm imposed by gdb. Also a MacsBug-like UI is supported (use the MB command to turn on the MacsBug screen). 

  * The _$dot_ gdb variable is the last address referenced by certain commands. This corresponds to Mac Classic's MacsBug '**.** ' variable. For example, **sm** sets _$dot_ to the first address that was changed. The default for many commands is to use _$dot_ as their address argument. Typing **dm** will display the memory just set and set the last command to **dm**. A return after the parameterless **dm** command will use the _$dot_ set by it to display then next block of memory in sequence. Note that this is the normal MacsBug behavior but that is different from gdb which would repeat the last command. 

  * The _$colon_ gdb variable is the address of the start of the proc containing the current PC when the target is running. 

  * Only C/C++ can be debugged since the gdb commands use C/C++ syntax for their implementation (restriction imposed by this implementation's use of gdb). 

  * Arguments are written using C/C++ syntax (e.g., use -> instead of ^, != instead of <>, etc.). 

  * Only one command per line allowed (gdb restriction). 

  * Some restrictions on the commands that are supported. Do **help** on individual commands for details. 

  * Decimal values are shown without leading '#' and hex is shown with a '0x' prefix (except in disassemblies and other displays where it is obvious it is hex). 

  * The input radix is as defined by the gdb **set input-radix** command. The default is decimal unlike MacsBug which defaults to hex. Use the gdb command **show input-radix** to verify the input radix. 

  * Many of the commands are not permitted until there is a target program running (using the MacsBug **g** or gdb **run** command). Most of the commands will cause some kind of error report until there is a target running. Of course with Mac Classic MacsBug there is always something running. But that's not the case with gdb. 

  * Some of the MacsBug **set** options are supported. Use **help support** to see a summary of **set** commands. All the MacsBug **set** options start with "mb-" so you can use standard command completion (hitting tab after typing the "mb" will show you all those **set** options. For compatibility with Mac Classic MacsBug, **set ditto** and **set unmangle** are also supported. 

  * Some Mac Classic MacsBug commands like **sc6** , **sc7** , and **sc** or **ilp** and **il** are identical in the gdb implementation. These alternatives take the form of gdb aliases. Use **help aliases** to see the supported aliases. 

  * **Si** is defined as the MacsBug **s** command since gdb already has a **s** command. But this overrides gdb's **si** abbreviation for **stepi**. 

  * When the MacsBug screen is being used command lines may be continued (using '\') within the area defined for commands. The default number of lines is 2 but may be increased up to 15 using the **set mb-cmd-area** command. Continuing more than the allotted space is allowed but will cause the command lines to scroll up within their area. Unfortunately this does not happen if a command is interactively created with **define** and you try to continue one of the command's lines. The continuations are allowed. But they just don't scroll. 

  * Unlike Mac Classic MacsBug, there is only one screen for gdb and the target program. Further, gdb has no control over output generated by the target program or caused by it (e.g., dyld messages). Thus the MacsBug screen could get "mangled". Use the **refresh** command to restore the MacsBug screen or turn off the MacsBug screen and debug with gdb's normal scroll mode. All the MacsBug commands still work in scroll mode and any commands that cause disassembly will display the current registers on the right side of the terminal screen (to minimize the affect on scroll back viewing). 

  * A set of possibly useful commands for extending this script are available. Typing **help useful** will list those commands. You will see some output commands that write their output to stderr instead of stdout. This is only significant in the context of the MacsBug screen. When the MacsBug screen is on (**mb on**) all output goes into the history area. Output to stderr is written in red. By convention this is be used for error reporting only. 



**6\. Terminal Considerations**

Gdb MacsBug assumes it is writing to a terminal window that should be
configured as follows:

  * Xterm enabled
  * VT100 or VT220 emulation
  * 8-bit connections
  * Window size of at least 44 rows by 80 columns if the MacsBug screen is turned on
  * Allows bolding of characters
  * Supports colors red and blue
  * Should use a mono-spaced font
  * No translation
  * Recommended font is Monaco 9 or at least a small enough font to fit within the MacsBug screen 44-line limitation

With the exception of **bolding** , the OSX standard terminal window supports
all these characteristics (use its preferences to configure for extra
bolding). The only colors MacsBug uses are red and blue. All other output uses
the default font color.

Color and bolding are, of course, not a strict requirement. But Xterm, 8-bit
support is. While an OSX standard terminal has no problems with these
requirements you do need to verify them when using a Telenet program.

Finally, different terminal programs as well as the OSX terminal window itself
have their own unique "quirks" and timing considerations or communication
failures may mangle the display. If this should happen while the MacsBug
screen is up you will need to type a **refresh** command to restore the proper
display.



**7\. Changes in MacsBug 1.6**

  * Fixed the MacsBug **set** and **show** commands (broken by incompatibilities with gdb-477). 

  * Fixed the MacsBug screen display errors (same reason). 

  * Fixed a infinite loop handing nested '<'s in function names (e.g., C++ templates) during disassemblies to the MacsBug screen. 

  * Fixed a bug where the MacsBug screen input (command) line was being erased when backspacing over the first character (if the line was beyond a minimum length). 

  * Some 64-bit and other internal bug fixes. 

  * New **set** mb-objc-selectors [ON|OFF|NOW|SHOW] command to display the selector of a Objective C message dispatch call in disassemblies if the call is at the current PC. Default is ON. 

  * If **set** print demangle, **set** print asm-demangle, or **set** [mb-]unmangle is ON then MacsBug attempts to demangle symbols if present in the comment field of disassmbled lines if they are not already demangled. For exmaple, C++ calls to symbols prefixed with "dyld_stub_" are not shown demangled in gdb. 

  * **set** mb-wrap now controls the wrapping of disassembled lines when the MacsBug screen of off in addition to its already existing control of the MacsBug screen history area lines. 



**8\. Changes in MacsBug 1.5**

  * Fixed crashing bug when attempting to use the MacsBug screen (**mb** command) that occurred with XCode 1.2. 

  * Fixed a bug where attempting to scroll through gdb's command history would overwrite the side bar. 

  * Fixed bug where the macsbug screen prompt was being garbled when resuming gdb after typing ^Z (control-Z) or changing the terminal window size while the inferior is currently executing. 

  * Resuming from ^Z or a window size change while the inferior is running interrupts the inferior's execution exactly as if a ^C was typed. 

  * Fixed bug in **sb** , **sw** , **sl** , and **sm** commands when they modify somthing showing in the macsbug screen pc area and the area was not being updated to reflect the modification. 

  * Fixed bug where the branch taken information (shown in the MacsBug screen when the pc is on a conditional branch) may be incorrect if it is the first break in the gdb session. 

  * Fixed bug to keep readline's ^R search "(reverse-i-search)" prompt from overwriting the sidebar. 

  * Fixed bug defining _$colon_ (current function start address) and made much more efficient. 

  * In the MacsBug screen side bar, if the program name for the "CurApName" field cannot be determined then the PID is shown instead. 

  * Added support for 64-bit architecture. MacsBug will determine whether the inferior is 32-bit or 64-bit and adjust its displays accordingly. 

  * New **set** mb-arch [32|64] command to force the displays to the desired mode independent of the actual inferior mode. Omitting the setting, i.e., **set** mb-arch resets to the inferior's mode. 

  * New commands **dll** , **sll** , and **fll** to display, set, and fill (respectively) long long (64-bit) values. 

  * New **dm** basic type keywords LongLong, SignedLongLong, and Binary64. 

  * **F** can be now be used as an abbreviation for **find** or **frame**. Abbreviated as **f** [arg] implies a **frame** command while **f** _addr n expr_ is interpreted as a **find** command. The only exception to this abbreviation rule is with the **help** command where you need to enter **help frame** to see that command's help display. 

  * The **wh** command will, if the line information is unavailable, display load segment information (which includes the segments type, address range, and, depending on type, either the load address or pathname). 



**9\. Changes in MacsBug 1.4**

  * Fixed bugs in **tf** and **tv**. 

  * Improved register display efficiency which greatly improves overall stepping speed. 



**10\. Changes in MacsBug 1.3**

  * Fixed typeo in the **fb** help info in the reference to **future-break**. 

  * Fixed memory write commands (e.g., **sb** , **sw** , etc.) to detect when the memory is inaccessible. 



**11\. Changes in MacsBug 1.2**

  * Fixed all commands that potentially bcak up over gdb prompt to produce a contiguous display to ensure the line is cleared before writing. Previously, under certain conditions, a portion of the gdb prompt would remain. Commands affect are: list, next, step, nexti, stepi, db, dw, dl, dm, dma, dp, id, il, ip. 

  * Added two new **set** commands: 

**set** mb-hexdump-width _w_  
**set** mb-hexdump-group _g_

These control the hexdump display width and byte grouping (e.g., the output of
the DM command). The width is the number of bytes shown per line. The grouping
is the number of bytes shown without intervening spaces. The default is a
width of 16 bytes per line in 4 groups of 4 bytes each. The width must be a
multiple of the group. Previously the width was fixed at 16 in groups of 2.



**12\. Changes in MacsBug 1.1**

  * Removed all references to the install-MacsBug script which was documented but never installed in 1.0. 

  * Changed plugin name from MacsBug to MacsBug_plugin which seems more appropriate. If you have your own copy of gdbinit-MacsBug you will have to change the **load-plugin** command (or continue naming the plugin with its original name). 

  * Removed the MSR and MQ registers from the sidebar. Thus the minimum screen size is 44 lines. 

  * Added bullet to Terminal Considerations above noting that you need a small enough font (recommended Monaco 9) to fit within the 44-line MacsBug screen limitation (the default OSX Monaco 10 did not allow the screen on iBooks). 

  * Fixed installation bug where the **load-plugin** pathname reflected the build directory as opposed to simply referencing /usr/libexec/gdb/plugins/MacsBug. 

  * Fixed **log** command to handle '~'s in the pathname. 

  * Fixed plugin code to more gracefully coexist with Project Builder. There were redirection interactions between Project Builder and the plugin. 

  * Added tests in key points to prevent MacsBug from doing xterm operations when not talking to a terminal as in the case with Project Builder. 

  * Fixed a bug in the **td** command which had a format error displaying the XER SOC bits. 

  * Added fixes so that the **mb-notes** help defined by **document** in gdbinit-MacsBug displays correctly if **mb-notes** is used explicitly in Project Builder's gdb. The **mb-notes** as defined in gdbinit-MacsBug has leading option-spaces to keep gdb's **document** reading from skipping leading blanks and all blank lines. These work fine on a terminal but do "funny things" to Project Builder's display of them. 

  * Added recognition of all the abbreviations for the **list** command to allow these to produce contiguous listings when the MacsBug screen is off. Previously it only worked for **l** and **list**. 

  * Made **next** , **step** , **nexti** , and **stepi** output be contiguous when repeatedly specified with no arguments when the MacsBug screen is off. 

  * Fixed bug in all contiguous listing commands (when MacsBug screen is off) when run from gdb **define** commands. Backup over gdb prompt should not be done under such conditions. 

  * Enhanced the MacsBug **wh** command to display file/line information along with the source line if possible. Syntax of argument made same as gdb's **info line**. 

  * Fixed bug in register display to adjust the stack display to account for a changed window size. 

  * Fixed scroll mode disassembly displays to wrap lines instead of being chopped off by register sidebar. 

  * Added **set mb-sidebar** mode setting command to enable/disable the display of the register sidebar when the **id** , **il** , **ip** , **so** , or **si** commands are done in gdb scroll mode. 

  * Moved **tf** from gdbinit-MacsBug to the plugin to allow the display to be more MacsBug-like. 

  * Implemented **tv**. 

