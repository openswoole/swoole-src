--TEST--
swoole_function: swoole_version
--SKIPIF--
<?php require __DIR__ . '/../include/skipif.inc'; ?>
--FILE--
<?php declare(strict_types = 1);
require __DIR__ . '/../include/bootstrap.php';

$version = swoole_version();
echo "swoole_version: $version";

?>
--EXPECTF--
swoole_version: %s
