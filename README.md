# lbase

A reimplementation of the GNU coreutils with a focus on speed, size, and
compatibility with GNU coreutils.

> NOTE: some redundant utilities will not be supported (e.g. `tac` instead of `rev`).
> If you want to change my mind, open an issue and I'll consider it.

## status

- [x] cat
	- supports `-u` (ignores)
	- does not (and will not) support `-abeEnstTv`
	- faster when reading small files, slower when reading larger files.
- [ ] tac
	- will not be supported.
- [ ] nl
- [ ] od
- [ ] base32
- [ ] base64
- [ ] basenc
- [ ] fmt
- [ ] pr
- [ ] fold
- [ ] head
- [ ] tail
- [ ] split
- [ ] csplit
- [ ] wc
- [ ] sum
- [ ] chsum
- [ ] b2sum
- [ ] md5sum
- [ ] sha1sum
- [ ] sha2
- [ ] sort
- [ ] shuf
- [ ] uniq
- [ ] comm
- [ ] ptx
- [ ] tsort
- [ ] cut
- [ ] paste
- [ ] join
- [ ] tr
- [ ] expand
- [ ] unexpand
- [ ] ls
- [ ] dir
- [ ] vdir
- [ ] dircolors
- [ ] cp
- [ ] dd
- [ ] install
- [ ] mv
- [ ] rm
- [ ] shred
- [ ] mkdir
- [ ] rmdir
- [ ] unlink
- [ ] mkfifo
- [ ] mknod
- [ ] ln
- [ ] link
- [ ] readlink
- [ ] chgrp
- [ ] chmod
- [ ] chown
- [ ] touch
- [ ] df
- [ ] du
- [ ] stat
- [ ] sync
- [ ] truncate
- [ ] echo
- [ ] printf
- [ ] yes
- [ ] false
- [ ] true
- [ ] test
- [ ] expr
- [ ] tee
- [ ] dirname
- [ ] basename
- [ ] pathchk
- [ ] mktemp
- [ ] realpath
- [ ] pwd
- [ ] stty
- [ ] printenv
- [ ] tty
- [ ] id
- [ ] logname
- [ ] whoami
- [ ] groups
- [ ] users
- [ ] who
- [ ] date
- [ ] arch
- [ ] nproc
- [ ] uname
- [ ] hostname
- [ ] hostid
- [ ] uptime
- [ ] chcon
- [ ] runcon
- [ ] chroot
- [ ] enc
- [ ] nice
- [ ] nohup
- [ ] stdbuf
- [ ] timeout
- [ ] kill
- [ ] sleep
- [ ] factor
- [ ] numfmt
- [ ] seq

I'm probably missing some. Let me know in the issues, and I'll mention it here.
