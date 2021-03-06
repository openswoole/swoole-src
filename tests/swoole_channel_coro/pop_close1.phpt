--TEST--
swoole_channel_coro: pop close 1
--SKIPIF--
<?php require __DIR__ . '/../include/skipif.inc'; ?>
--FILE--
<?php declare(strict_types = 1);
require __DIR__ . '/../include/bootstrap.php';

$c1 = new chan();
$c1->close();

go(function () use ($c1) {
    $ret = $c1->pop();
    echo "pop ret:".var_export($ret, true)." error:".$c1->errCode."\n";
});
?>
--EXPECTF--
pop ret:false error:-2
