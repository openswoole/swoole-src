<?php
Swoole\Runtime::enableCoroutine();
Co\run(function () {
    $fp = fopen('data.txt', 'w+');
    fwrite($fp, str_repeat('A', 8192));
    fflush($fp);
    fclose($fp);
});
