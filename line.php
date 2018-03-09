<?php
<!doctype html>
<html>
<head>
	<html lang="ja">
	<meta charset="utf-8">
	<title>自動販売機迷子捜索システム-TOP</title>
	<link href="style.css" rel="stylesheet" type="text/css" media="screen and (min-width:601px)">
	<link href="style_2.css" rel="stylesheet" type="text/css" media="screen and (max-width:600px)">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<meta name="description" content="自動販売機迷子捜索システム">
</head>
<body>
	<header>
		<div id="title"></div>
	</header>

	<main>
		<a href="https://access.line.me/dialog/oauth/weblogin?response_type=code&client_id=1567087773&redirect_uri=https://aonr.jp/app/line.html">
		<img src="img.jpg" width="20%">
		<script>
   			var postedData = <?php echo($_POST['code']); ?>;
   			alert(postedData);
		</script>
		<textarea name="postedData"></textarea>
		</a>
		<p><a href="TOP.html">TOPへ</a></p>

		 <h2>「ここに」<?php $_POST['code'] ?>「表示」</h2>
	</main>

	<footer>

	</footer>

</body>
</html>

?>