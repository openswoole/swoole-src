--TEST--
swoole_process: exec
--SKIPIF--
<?php
require __DIR__ . '/../include/skipif.inc';
if (!@file_exists('/usr/bin/python')) {
    exit('skip if no python');
}
?>
--FILE--
<?php declare(strict_types = 1);
require __DIR__ . '/../include/bootstrap.php';

$process = new swoole_process('python_process', true);
$pid = $process->start();

function python_process(swoole_process $worker)
{
    $worker->exec('/usr/bin/python', array(__DIR__ . "/echo.py"));
}

$process->write("Hello World\n");
echo $process->read();
?>
Done
--EXPECTREGEX--
Python: Hello World
Done.*
