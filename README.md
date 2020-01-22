# lbase

A reimplementation of the GNU coreutils with a focus on speed and size.

**notes**:
- some redundant utilities will not be supported (e.g. `tac` instead of `rev`).
- certain flags will not be supported (e.g. `cat -v`).
- no attempt is currently being made at being POSIX-compliant. The goal is
to get these tools working properly first.
- UTF8 support is primitive at best. This will be fixed later.

If you want to change my mind, open an issue and I'll consider it.

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
- [x] head
	- doesn't support `head -n -123` (yet).
	- doesn't (and won't) support `--zero-terminated`.
- [ ] tail
- [ ] split
- [ ] csplit
- [x] wc
	- **x25** faster than GNU `wc`
	- does not (and won't) support `--files0-from`
	- doesn't properly support `--max-line-width` (yet).
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
- [x] yes
	- 1/6 as small as GNU `yes`, and twice as fast.
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
