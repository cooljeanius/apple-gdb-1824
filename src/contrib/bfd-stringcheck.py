from __future__ import print_function
import re
from optparse import OptionParser
from pyg3t.gtparse import iparse

p = OptionParser()
opts, args = p.parse_args()

pattern = re.compile(r'%[AB]')

warnings = {}
nmsgs = {}

for arg in args:
    cat = iparse(open(arg, 'rb'))
    nwarn = 0
    nmsg = 0
    for msg in cat:
        if not msg.istranslated:
            continue
        msgidtokens = pattern.findall(msg.msgid)
        msgstrtokens = pattern.findall(msg.msgstrs[0])
        nmsg += 1
        if msgidtokens != msgstrtokens:
            print('%s L%d' % (arg, msg.meta['lineno']))
            print('  %s' % ' '.join(msgidtokens))
            print('  %s' % ' '.join(msgstrtokens))
            print(msg.tostring().encode('utf8'))
            nwarn += 1
    warnings[arg] = nwarn
    nmsgs[arg] = nmsg
    print('%s:%d warnings' % (arg, nwarn))

print()
print('Summary')
print('=======')
for arg in args:
    print('%s: %d warnings / %d messages' % (arg, warnings[arg], nmsgs[arg]))
