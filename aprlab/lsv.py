from subprocess import *
from os import *

def init():
	system('rm -f ~/.ssh/known_hosts')
	system('echo "StrictHostKeyChecking no" > ~/.ssh/config')
	# https://help.github.com/articles/generating-ssh-keys
	# ssh-copy-id user@machine

def upload(srcFile, remoteHost, dstDir, userName='root'):
	dest = ''.join([userName,'@',remoteHost,':',dstDir])
	p = Popen(["scp", srcFile, dest])
	p.wait()
	return p.returncode
