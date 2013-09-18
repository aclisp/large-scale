#! /usr/bin/env python
CONFIG = {
	"hostname" : "127.0.0.1",
	"port"     : "80",
}

def func():
	print CONFIG["hostname"]
	CONFIG["hostname"] = "localhost"

# -- main start --
func()
print CONFIG["hostname"]

# http://www.saltycrane.com/blog/2008/01/python-variable-scope-notes/
