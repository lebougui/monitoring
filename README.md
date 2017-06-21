# Compilation

```
gcc monit.c  -o monit -O2 -Wall -W -Werror  -pedantic
```

# Tests

``` 
./monit -m /proc/
/proc/ stats :
type of filesystem = 0x9fa0 (PROC_SUPER_MAGIC),
optimal transfer block size = 0x1000
total data blocks in file system = 0x0

free blocks in fs = 0x0
free blocks avail to non-superuser = 0x0
total file nodes in file system = 0x0
free file nodes in fs = 0x0
maximum length of filenames = 0xff

[/proc/ access monitoring]	Date=2017-06-21 12:00:34	Object=interrupts	Event=IN_OPEN(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:34	Object=interrupts	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:34	Object=stat	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:44	Object=interrupts	Event=IN_OPEN(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:44	Object=interrupts	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:44	Object=interrupts	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:44	Object=stat	Event=IN_OPEN(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:44	Object=stat	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:54	Object=interrupts	Event=IN_OPEN(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:54	Object=interrupts	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:54	Object=stat	Event=IN_OPEN(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:54	Object=stat	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:00:54	Object=stat	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:01:04	Object=interrupts	Event=IN_OPEN(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:01:04	Object=interrupts	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:01:04	Object=stat	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:01:14	Object=interrupts	Event=IN_OPEN(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:01:14	Object=interrupts	Event=IN_ACCESS(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:01:24	Object=interrupts	Event=IN_OPEN(FILE)
[/proc/ access monitoring]	Date=2017-06-21 12:01:34	Object=interrupts	Event=IN_OPEN(FILE)
....

```


