/*
 * DataFormatterPlugin.h
 */

#include <stdarg.h>

/*
 Xcode supports customizing the information displayed in the debugger data view.
 Developers can enter simple format strings which describe what to display in
 the variable value and summary columns on a per-type basis. These format
 strings are described below. Additionally, developers can write bundles which
 are loaded into the debugged process. This allows developers to write their
 own "print for debugger" functions which can be invoked via the format strings.

 A format string consists of literal text, references to child data values of a
 structured variable (as they appear in the outline view), and/or arbitrary
 expressions, in any combination. This lets the user build a custom string
 consisting of interesting fields and properties of a variable. The user edits
 the format string by double clicking the appropriate column for any variable.

 A child reference looks like:

 %childPath%:referencedColumn

 where childPath is a . delimited "path" to the child data value being
 referenced (A numeric reference is also supported, so format strings can refer
 to the n'th child rather than naming the child.) The % characters bracket the
 child path. The referencedColumn is one of the characters n, v, t, or s,
 indicating to use text from the variable name, value, type name, or summary
 entry of the referenced child. The referencedColumn and it's preceding colon
 can be omitted, in which case the value column is used as the default.

 Take for example the NSRect produced by NSMakeRect(1,2,3,4). The format string:

	x=%origin.x%, y=%origin.y%, width=%size.width%, height=%size.height%

 would product the display:

	x=0, y=1, width=2, height=3

 Or, another example, if the format strings for NSPoint, NSSize, and NSRect are
 (respectively):

	x=%x%, y=%y%
	width=%width%, height=%height%
	origin=(%origin%), %size%

 Then the display of the rect would be:

	origin=(0,1), width=2, height=3

 Note that the formatted display for the origin is used in the display for the
 NSRect. The format strings are "recursive".

 For C++ code it is not necessary to include the public, private, or protected
 entries in the child path. These "fake" children are automatically skipped.
 For example, in a C++ program the structure in the variable view might look
 like rect->public->origin->public->x. But to reference the child it is
 sufficient to write %origin.x%.

 The expression form in a format string looks like:

	{expression}:referencedColumn

 This is similar to the child reference, except that braces {} are used to
 bracket the expression rather than % characters. The referencedColumn is the
 same as for a child reference (above). Inside an expression, the text $VAR is
 replaced with an expression for referencing the variable itself. Thus, the
 equivalent expression for the first NSRect example above would be:

	x={$VAR.origin.x}, y={$VAR.origin.y}, width={$VAR.size.width}, height={$VAR.size.height}

 This is a relatively simplistic example, as there is no computation associated
 with these expressions, and in such a case the child referencing form is more
 efficient. But any expression can theoretically be used, which could include
 function calls. So for a NSArray you might have a formatter like:

	count={(int)[$VAR count]}

 This format string would show the count of objects in the array. Note that
 dynamic expressions must be evaluated every time the process stops, including
 after each step. So format strings which are computationally expensive can
 degrade stepping time.

 There are some additional special symbols (besides $VAR) that can be used in
 an expression. The complete list and their meanings is:

 $VAR - replaced with an expression evaluating to the variable itself
 $ID - replaced with an identifier which can be passed to a plugin function
       (see below)
 $PARENT(type) - replaced with an expression for the parent of the variable
                 which is of the given type. This is only useful for a very
                 limited circumstance, where values of a particular type are
                 always contained in a structure of known type, and information
                 from the containing structure is required to interpret the
                 value. For example, an "event" structure might contain an
                 "eventType" code, and an "eventResult" value, where the
                 eventResult values are overloaded for different eventTypes.
                 This construct provides a mechanism for the eventResult
                 formatter to obtain the containing event to access the
                 eventType.

 Custom format strings are stored and looked up on a per type basis. So if you
 set a format string for a given data variable, all other variables with the
 same type will get the same format string. Type attributes are stripped from
 the type name, so "const NSRect", "volatile NSRect", and plain "NSRect" all
 get the same formatters.

 Custom format strings are loaded from several locations. Xcode searches some
 well-known directories for .plist files and bundles which contain custom format
 strings. These are (in highest to lowest precedence):

     ~/Library/Application Support/Apple/Developer Tools/CustomDataViews (obsolete)

 And now, after <rdar://6696138>
     ~/Library/Application Support/Developer/Shared/Xcode/CustomDataViews
     ~/Library/Application Support/Developer/<Xcode_release>/Xcode/CustomDataViews
     /Library/Application Support/Developer/Shared/Xcode/CustomDataViews
     /Library/Application Support/Developer/<Xcode_release>/Xcode/CustomDataViews

     ${DEVELOPER_DIR}/Library/Xcode/CustomDataViews


 If there are multiple format strings defined for the same type, the highest
 precedent one is used. At present there is no mechanism for selecting from
 among multiple possible formatters for a particular variable.

 If the user makes changes to a format string in the GUI, the changes are
 always saved in one file:

     ~/Library/Application Support/Developer/Shared/Xcode/CustomDataViews/CustomDataViews.plist

 This file has the highest precedence so user customizations are always used.

 The developer tools package includes some built in formatters for Foundation
 and Carbon data types installed in ${DEVELOPER_DIR}/Library/Xcode/CustomDataViews.
 These can of course be further customized by developers, either by editing the
 provided format strings or by overriding them in the GUI. (At present, however,
 there is no way to completely remove a built in format string using the GUI,
 because deleting a user's customization results in the built in version being
 used.)

 The .plist file which contains format strings is a simple dictionary. The keys
 are type strings and the values are formatter dictionaries. Additionally, the
 dictionary has a key named "File Version" whose value is the file format
 version number (currently 1).

 For the version 1 file format each formatter dictionary has two key/value
 pairs. The keys are "ValueString" and "SummaryString", and the values are the
 custom formatter strings as described above.

 Creating a new .plist file is relatively straightforward. Formatter strings
 can be entered directly into the GUI by double clicking on the value or
 summary columns. This will store the required entries into the user's
 customized plist (~/Library/Application Support/Developer/Shared/Xcode/CustomDataViews).
 The appropriate entries can be copied to a .plist file with a different name
 in any of the above locations and it will be loaded the next time Xcode is
 restarted.

 However, for some types there is no combination of simple expressions which
 can construct a suitable description. For these types there is support for
 writing custom code to construct the descriptions. Developers can create a
 bundle which contains functions they write themselves to construct display
 strings. The bundle must also contain a file resource named CustomDataViews.plist,
 which is the plist containing format strings. Whenever a formatter string from
 the bundle's .plist is used, Xcode ensures that the bundle code is loaded
 into the process being debugged. The expressions in the format strings can
 contain calls to functions in the bundle. The functions you write should be as
 lightweight as possible because they will be evaluated after every step in the
 debugger. Additionally they should not modify any global state in the process.
 And they must be carefully written to avoid causing problems when presented
 with uninitialized variables. gdb can typically handle the case where a
 function called from an expression causes a crash, but it is good to be as
 prudent as possible.

 A bundle to support custom data formatting must define the following symbol:

 _pbxgdb_plugin_function_list *_pbxgdb_plugin_functions = NULL;

 This must be defined as a global symbol in the bundle. The bundle is loaded
 using NSLINKMODULE_OPTION_PRIVATE, so this symbol (and any other globals) are
 not merged into the global symbol table of the program. When the bundle is
 loaded the loading code looks up this symbol by name and fills in a value.
 This allows the plugin to use utility functions which are already present in
 the debugging environment without requiring that the linker be informed of
 them. These functions are described below. (Note that bundle initializer
 functions are called before this value is filled in.)
 */


/*
 Memory allocator function.
 Typically a plugin function will return a string value, so the function needs
 memory to store the string. Typically there may be more than one variable of a
 given type in scope, so a static buffer cannot be used. Also, Xcode reads the
 value of the string asynchronously, so the plugin cannot know how long the
 buffer needs to be valid. Therefore, the Xcode debugging environment provides
 a special memory allocator. This allows Xcode to manage the memory allocated
 by a plugin.
 The value for the identifier parameter is supplied by the expression parser in
 Project Builder using "$ID". This value should be passed to the allocator.
 Only one memory allocation is allowed per identifier. If a buffer for the
 identifier already exists, the existing buffer will be reused if it is large
 enough. Otherwise the old buffer is freed and a new, larger one is allocated.
 The size parameter specifies the minimum size in bytes that the returned
 buffer must be able to contain.
 The memory buffer returned by this allocator should not be freed by the plugin.
 The lifetime of this buffer is determined by Xcode.
 (The plugin can call malloc() and free() if necessary. But this should be
 avoided if possible.)
 Note that returning a literal string does not require any runtime allocation.
 */
typedef void *(*_pbxgdb_allocator)(int identifier, int size);

/*
 Allocate and construct a formatted character string.
 This function is a convenience function which allocates a buffer using the
 _pbxgdb_allocator function, then constructs a character string in the
 allocated buffer using a printf() style format string and vararg list.
 The value for the identifier parameter is supplied by the expression parser
 in Project Builder using "$ID". This value is passed to the allocator.
 The format string and additional arguments are passed to vsnprintf(). Refer to
 the man page for more about printf() format strings.
 */
typedef char *(*_pbxgdb_message)(int identifier, const char *format, ...);
typedef char *(*_pbxgdb_vmessage)(int identifier, const char *format, va_list ap);

typedef unsigned short *(*_pbxgdb_unicharmessage)(int identifier, const char *format, ...);
typedef unsigned short *(*_pbxgdb_unicharvmessage)(int identifier, const char *format, va_list ap);


/*
 This structure defines a list of function pointers which can be called by
 functions in a plugin bundle. Functions may be added to this list in future
 releases, but functions will never be removed from the list, nor will the
 signature of existing functions ever change. However, individual functions may
 be deprecated and removed from the runtime. In this case the entry in the list
 will remain but will be NULL. Well-behaved plugins should verify that the
 function pointers are non-NULL before calling through them, and return a
 suitable error message in that case.
 */
typedef struct {
    _pbxgdb_allocator   allocate;
    _pbxgdb_message     message;
    _pbxgdb_vmessage    vmessage;
    _pbxgdb_unicharmessage unicharmessage;
    _pbxgdb_unicharvmessage vunicharmessage;
} _pbxgdb_plugin_function_list;

/* EOF */
