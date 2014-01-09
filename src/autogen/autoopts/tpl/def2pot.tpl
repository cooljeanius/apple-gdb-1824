[= AutoGen5 template pot                                     =][=
#
# this template can be used to generate .pot file for the
# option definition files for these templates:
#       aginfo.tpl, agman-cmd.tpl, agmdoc-cmd.tpl
#

====================== FUNCTIONS BEGIN =======================][=
DEFINE genmsg                                                =][=

  IF (string-length (get (get "msgid")))                     =]
#: [=(def-file-line (get "msgid") "%s:%d")                   =]
msgid [= (c-string (get (get "msgid")))                      =]
msgstr ""
[=ENDIF                                                      =][=
ENDDEF                                                       =][=

DEFINE genmsg2                                               =][=
  IF (string-length (get "msgid"))                           =]
#: [=(def-file-line (get "msgid") "%s:%d")                   =]
msgid [= (c-string (get "msgid"))                            =]
msgstr ""
[=ENDIF                                                      =][=
ENDDEF                                                       =][=
# ==================== FUNCTIONS END ============================

# pot file header and comment info                          \=]
# localization template (.pot) for [= def-file =] of [= prog-name =],
# this file is used to generate localized manual for [= prog-name =].
# Copyright (C) [= (shell "date +%Y")                        =]
# This file is distributed under the same license as the [= prog-name =] package.
# FIRST AUTHOR <EMAIL@ADDRESS>, [= (shell "date +%Y") =].
#
#, fuzzy
msgid   ""
msgstr  "Project-Id-Version: [= prog-name =] [= version =]\n"
        "Report-Msgid-Bugs-To: \n"
        "POT-Creation-Date: [= (shell "date +\"%F %R%z\"") =]\n"
        "PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\n"
        "Last-Translator: FULL NAME <EMAIL@ADDRESS>\n"
        "Language-Team: LANGUAGE <LL@li.org>\n"
        "Language: \n"
        "MIME-Version: 1.0\n"
        "Content-Type: text/plain; charset=CHARSET\n"
        "Content-Transfer-Encoding: 8bit\n"
[=
genmsg msgid=prog-title                                      =][=

FOR flag                                                     =][=
  genmsg msgid=descrip                                       =][=
  genmsg msgid=doc                                           =][=
ENDFOR                                                       =][=

FOR explain                                                  =][=
  genmsg msgid=explain                                       =][=
ENDFOR                                                       =][=

FOR doc-section                                              =][=
  genmsg msgid=ds-text                                       =][=
ENDFOR                                                       =][=

FOR prog-man-descrip                                         =][=
  genmsg msgid=prog-man-descrip                              =][=
ENDFOR                                                       =][=

FOR prog-info-descrip                                        =][=
  genmsg msgid=prog-info-descrip                             =][=
ENDFOR                                                       =][=

FOR detail                                                   =][=
  genmsg msgid=detail                                        =][=
ENDFOR                                                       =][=

FOR exit-desc                                                =][=
  genmsg msgid=exit-desc                                     =][=
ENDFOR                                                       =][=

CASE (get "copyright.type")                                  =][=
  = note                                                     =][=
  == ''                                                      =][=
  *                                                          =][=
    genmsg2 msgid=(string-append
        "This program is released under the terms of "
        (license-name (get "copyright.type")) ".")
                                                             =][=
ESAC                                                         =][=

genmsg msgid=option-info                                     =][=
genmsg msgid=argument                                        =][=
genmsg msgid=man-doc                                         =][=
genmsg msgid=copyright.text                                  =]
