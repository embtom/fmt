#!/usr/bin/env python3
# Build the documentation.

from __future__ import print_function
import errno, os, shutil, sys, tempfile
from subprocess import check_call, check_output, CalledProcessError, Popen, PIPE

versions = ['1.0.0', '1.1.0', '2.0.0', '3.0.2', '4.0.0', '4.1.0', '5.0.0', '5.1.0', '5.2.0', '5.2.1', '5.3.0', '6.0.0']

def build_docs(version='dev', **kwargs):
  doc_dir = kwargs.get('doc_dir', os.path.dirname(os.path.realpath(__file__)))
  work_dir = kwargs.get('work_dir', '.')
  include_dir = kwargs.get(
      'include_dir', os.path.join(os.path.dirname(doc_dir), 'include', 'fmt'))
  # Build docs.
  cmd = ['doxygen', '-']
  p = Popen(cmd, stdin=PIPE)
  doxyxml_dir = os.path.join(work_dir, 'doxyxml')
  p.communicate(input=r'''
      PROJECT_NAME      = fmt
      GENERATE_LATEX    = NO
      GENERATE_MAN      = NO
      GENERATE_RTF      = NO
      CASE_SENSE_NAMES  = NO
      INPUT             = {0}/core.h {0}/format.h {0}/ostream.h \
                          {0}/printf.h {0}/time.h
      QUIET             = YES
      JAVADOC_AUTOBRIEF = YES
      AUTOLINK_SUPPORT  = NO
      GENERATE_HTML     = NO
      GENERATE_XML      = YES
      XML_OUTPUT        = {1}
      ALIASES           = "rst=\verbatim embed:rst"
      ALIASES          += "endrst=\endverbatim"
      MACRO_EXPANSION   = YES
      PREDEFINED        = _WIN32=1 \
                          FMT_USE_VARIADIC_TEMPLATES=1 \
                          FMT_USE_RVALUE_REFERENCES=1 \
                          FMT_USE_USER_DEFINED_LITERALS=1 \
                          FMT_USE_ALIAS_TEMPLATES=1 \
                          FMT_API= \
                          "FMT_BEGIN_NAMESPACE=namespace fmt {{" \
                          "FMT_END_NAMESPACE=}}" \
                          "FMT_STRING_ALIAS=1" \
                          "FMT_ENABLE_IF(B)="
      EXCLUDE_SYMBOLS   = fmt::internal::* StringValue write_str
      FULL_PATH_NAMES   = NO
    '''.format(include_dir, doxyxml_dir).encode('UTF-8'))
  if p.returncode != 0:
    raise CalledProcessError(p.returncode, cmd)
  html_dir = os.path.join(work_dir, 'html')
  main_versions = reversed(versions[-3:])
  check_call(['sphinx-build',
              '-Dbreathe_projects.format=' + os.path.abspath(doxyxml_dir),
              '-Dversion=' + version, '-Drelease=' + version,
              '-Aversion=' + version, '-Aversions=' + ','.join(main_versions),
              '-b', 'html', doc_dir, html_dir])
  try:
    check_call(['lessc', '--clean-css',
                '--include-path=' + os.path.join(doc_dir, 'bootstrap'),
                os.path.join(doc_dir, 'fmt.less'),
                os.path.join(html_dir, '_static', 'fmt.css')])
  except OSError as e:
    if e.errno != errno.ENOENT:
      raise
    print('lessc not found; make sure that Less (http://lesscss.org/) ' +
          'is installed')
    sys.exit(1)
  return html_dir

if __name__ == '__main__':
  build_docs(sys.argv[1])
