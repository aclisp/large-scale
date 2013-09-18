#! /usr/bin/env python
from subprocess import *
p1 = Popen(["cat", "/etc/hosts"], stdout=PIPE)
p2 = Popen(["grep", "127.0.0.1"], stdin=p1.stdout, stdout=PIPE)
output = p2.communicate()[0]
print output,

# http://docs.python.org/2.6/library/subprocess.html#replacing-older-functions-with-the-subprocess-module
