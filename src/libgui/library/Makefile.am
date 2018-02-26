## Process this file with automake to produce Makefile.in.

AUTOMAKE_OPTIONS = 1.11.6 gnits no-dist dejagnu
## "cygnus" == "foreign no-dist" (we hardened from foreign to gnits though)
AUTOMAKE_DEPRECATED_OPTIONS = cygnus

## Convenience variables.
TCL = advice.tcl balloon.tcl bbox.tcl bgerror.tcl bindings.tcl \
canvas.tcl cframe.tcl center.tcl debug.tcl def.tcl internet.tcl	\
font.tcl gensym.tcl gettext.tcl hooks.tcl lframe.tcl list.tcl \
looknfeel.tcl menu.tcl mono.tcl multibox.tcl parse_args.tcl path.tcl \
postghost.tcl prefs.tcl print.tcl sendpr.tcl topbind.tcl toolbar.tcl \
ulset.tcl wframe.tcl wingrab.tcl ventry.tcl combobox.tcl \
pane.tcl panedwindow.tcl

PACKAGES = combobox.tcl

## This directory is also referenced in paths.c, which see.
guidir = $(datadir)/redhat/gui
gui_DATA = tclIndex pkgIndex.tcl $(TCL) @MAINT@ $(PACKAGES)

if TCL_SHARED
SET_LIB_PATH = 	$(RPATH_ENVVAR)=$$here/../../tcl/unix:$$here/../../itcl/itcl/unix:$$$(RPATH_ENVVAR); export $(RPATH_ENVVAR);
else
SET_LIB_PATH = 
endif

WISH = wish

if CROSS_COMPILING
ITCL_SH_TO_USE = itclsh3.0
else
ITCL_SH_TO_USE = @ITCL_SH@
endif

if MAINTAINER_MODE
tclIndex: $(TCL)
	TCL_LIBRARY=$(srcdir)/../../tcl/library; export TCL_LIBRARY; \
	here=`pwd`; \
	$(SET_LIB_PATH) \
	cd $(srcdir) && \
	  echo "auto_mkindex $(LIBGUI_LIBRARY_DIR) $(TCL)" | $(ITCL_SH_TO_USE)

pkgIndex.tcl: @MAINT@ $(PACKAGES)
	here=`pwd`;	 \
	$(SET_LIB_PATH) \
	cd $(srcdir) && \
	echo "pkg_mkIndex . $(PACKAGES); exit" | $(ITCL_SH_TO_USE)
else
tclIndex:

pkgIndex.tcl:

endif

ETAGS_ARGS = --lang=none --regex='/[ \t]*\(proc\|method\|itcl_class\)[ \t]+\([^ \t]+\)/\1/' $(TCL) --lang=auto
