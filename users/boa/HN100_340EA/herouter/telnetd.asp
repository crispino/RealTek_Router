<html>
<head>
<meta http-equiv="Content-Type" content="text/html">
<title>Telnet</title>
<script type="text/javascript" src="js/jquery.js"></script>

<script type="text/javascript">
$(document).ready(function(){
		$.ajax({
		type: "post",
		url: "/boafrm/heUITelnetd",
		data: "",
		success: function(data){
			$("#result").text("Success.");
		},
		error: function(data) {
			$("#result").text("Fail!!");
		}
	});
});

</script>
</head>
	<body>
		<span>Telnetd open: <em id="result">Success.</em></span>
	</body>
</html>
