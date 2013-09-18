#! /usr/bin/env python

def do_something(i):
    print i

for i in xrange(10):
    do_something(i)

lst = ["a", "b", "c"]

for item in lst:
    do_something(item)

# http://www.cafepy.com/article/be_pythonic/
