<?php
	$input = str_replace("\r", "", $_POST['inputprogram']);

	$arqbuffer = fopen("Easter_Egg----1235813-20", "w");
	fprintf($arqbuffer, "%s", $input);
	fclose($arqbuffer);

	echo shell_exec("./prolini Easter_Egg----1235813-20 ".$_POST["programtype"]);
?>
