<?php
	$curl_handle=curl_init();
	curl_setopt($curl_handle,CURLOPT_URL,'192.168.1.99');
	curl_setopt($curl_handle,CURLOPT_CONNECTTIMEOUT,2);
	curl_setopt($curl_handle,CURLOPT_RETURNTRANSFER,1);
	$buffer = curl_exec($curl_handle);
	curl_close($curl_handle);
	if (empty($buffer)){
		print "Nothing returned from url.<p>";
	}
	else{
		$first = substr($buffer, 0,2);
		$last = substr($buffer,2,2);
		$buffer = $first.','.$last."\u{00B0}C";
		print $buffer;
	}
?>