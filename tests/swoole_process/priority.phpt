--TEST--
swoole_process: priority
--SKIPIF--
<?php require __DIR__ . '/../include/skipif.inc'; ?>
--FILE--
<?php declare(strict_types = 1);
require __DIR__ . '/../include/bootstrap.php';

use Swoole\Process;

const PRIORITY = 12;

$process = new Process(function(Process $worker) {
    $worker->setPriority(PRIO_PROCESS, PRIORITY);
    $priority =  $worker->getPriority(PRIO_PROCESS);
    Assert::eq($priority, PRIORITY);
    usleep(20000);
    $worker->exit(0);
}, false, 0);

$pid = $process->start();
Process::wait();
?>
--EXPECT--
