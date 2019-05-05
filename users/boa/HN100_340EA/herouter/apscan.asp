<!doctype html> 
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>互联网设置</title>
<link href="css/mainstyle.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="js/herouter.js"></script>
<script type="text/javascript" src="js/jquery.js"></script>
<style type="text/css">
<!--
body{}
*{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
*:hover{-moz-transition: all 0.3s ease-in;-webkit-transition: all 0.3s ease-in;-o-transition: all 0.3s ease-in;transition: all 0.3s ease-in;}
.int { height:510px; width:670px; overflow:hidden; background:#fcfcfa url(images/fullbg.gif) repeat-x; padding:10px 15px;}
.int h1{ font-size:18px; line-height:42px; border-bottom:1px #d3d3d3 solid; height:50px;}
.int h1 strong{ float:left; padding-right:20px; color:#676767;}
.int b{display:block; height:42px; cursor:pointer; padding:0 15px;font-weight:normal; float:left; background:#77d1df; font-size:18px; color:#fff; text-align:center; line-height:42px;}
.int b:hover{ background:#56bbcb;}
.int .rb{ float:right; margin-left:10px;background:#99e4f0; }
.int .cancel{background:#99e4f0; }
.spc5{ margin-right:5px;}
.overview{ padding-top:5px; height:42px;}
.int .unselect{ background:#d2d2d2;}
.data{ float:right; color:#a3a3a3; width:332px;}
.moddata{ float:left; width:170px; height:42px;}
.moddata .txt{float:left; font-size:12px; height:42px; line-height:21px;}
.fh{float:left; font-size:42px; line-height:36px; height:42px; text-align:center; width:25px;}
.datashow{float:left; background:url(images/upanddown.png) no-repeat left center; height:39px; padding-left:20px; width:68px; color:#919191; line-height:18px; padding-top:3px;}
.alldata{ float:right; width:150px; height:42px; padding-left:12px; background:url(images/sls.png) no-repeat left center;}
.alldata .alltxt{float:left; line-height:42px; font-size:12px;}
.tabarrow{ height:9px; background:url(images/arrowtab.png) no-repeat 45px 5px; padding-top:5px;}
.tabarrow2{ height:9px; background:url(images/arrowtab.png) no-repeat 150px 5px; padding-top:5px;}
.datalist{ background:#fff; padding:7px;height:335px; overflow:hidden;}
.title{ height:35px; background:#f4f4f4;}
.title ul{}
.title li{ float:left; line-height:35px ; font-size:14px; color:#666;}
.title li span{ padding-left:35px;}
.l1{ width:260px;}
.l2{ width:15px;}
.l3{ width:120px;}
.l4{ width:100px;}
.l5{ width:70px;}
.l6{ width:30px; text-align:left;cursor:pointer;}
.data_detail{ }
.data_detail ul{ height:40px; padding:5px 0; border-bottom:1px #f1f1f1 solid;}
.data_detail li { float:left; height:40px; line-height:40px; color:#919191;}
.data_detail li span{ padding-left:35px; height:28px; margin:6px 0; display:block; line-height:28px; background:url(images/device.png) no-repeat }
.data_detail li .device1{ background-position:0 0;}
.data_detail li .device2{ background-position:0 -28px;}
.data_detail li .device3{ background-position:0 -56px;}
.data_detail li .device4{ background-position:0 -84px;}
.data_detail li .device5{ background-position:0 -114px;}
.data_detail li .device6{ background-position:0 -140px;}
.data_detail li p{ line-height:20px;}
.data_detail li b{ font-size:12px; line-height:20px; height:20px;}
.data_detail .l4{ background:url(images/upanddown.png) no-repeat 0 center; padding-left:20px; width:80px; line-height:20px;}
.data_detail li i{ font-style:normal; line-height:16px;}
.data_detail li strong{ display:block;width:40px; height:40px; font-size:14px; color:#77d1df; text-align:center; background:#f6f6f6; font-weight:normal; cursor:pointer;}
.data_detail li strong:hover{ background:#eaeaea;}
.data_detail li em{ font-style:normal; height:40px; width:70px; font-size:14px; background:#f6f6f6; cursor:pointer; display:block; text-align:center; color:#ff8484;}
.data_detail li em:hover{background:#eaeaea;}
.countinfo{ height:50px; line-height:50px;}
.countinfo ul{}
.countinfo li{ color:#008edb; float:left; font:14px;}
.countinfo li em{ font-size:18px; font-weight:bold; font-style:normal;}
.countinfo .pagechange{ font-size:18px; float:right; padding-top:15px; }
.countinfo strong{ font-weight:normal; margin-left:20px; cursor:pointer;}
.countinfo .nextpage{ background:url(images/randl.png) no-repeat right 0; padding-right:21px; line-height:21px;}
.countinfo .prepage{ background:url(images/randl.png) no-repeat left -21px; padding-left:21px; line-height:21px;}
.info{ padding:10px 0; height:26px;width 40%}
.info span{ float:left; line-height:26px; color:#666;}
.info b{ float:left; line-height:26px; height:26px; margin:0 10px; font-size:16px;}
.save {  float:right; line-height:50px; font-size:26px;height:50px;margin:0 10px;}
-->
</style>
<script type="text/javascript">
var startnum = 1;
var endnum   = 5;
function NextDisplay(Clientnum)
{
	var ID= new Array();
	var i = startnum;
	
	if(Clientnum >= 5 && endnum <Clientnum)
	{
		for(i;(i<=endnum+5)&&(i<=Clientnum) ;i++)
		{
				ID[i] ="display"+i	;
				if(i <= endnum)
				{
					document.getElementById(ID[i]).style.display = "none";
					startnum++;
				}
				else
				{	
					document.getElementById(ID[i]).style.display = "";
				}
		}
		endnum = i -1;
	}
}

function PreDisplay(Clientnum)
{
	var ID= new Array();
	var i = endnum < Clientnum ?endnum:Clientnum;
	if(startnum > 5&& endnum > 5)
	{
		for(i;i >= startnum-5;i--)
		{
				ID[i] ="display"+i	;
				if(i >= startnum)
				{
					document.getElementById(ID[i]).style.display = "none";
					endnum--;
				}
				else
					document.getElementById(ID[i]).style.display = "";
		}
		startnum = i+1;
	}
}

function backtowifisettings(num)
{
	var ID;
	var bssid, ssid, encType;
	var para;
	if(num!=-1)
	{
		ID= "index"+num	;
		bssid = document.getElementById(ID).innerHTML;		

		ID= "encType"+num;
		encType = document.getElementById(ID).innerHTML;		
			
		para='wifisetting1.asp?bssid='+bssid+'&encType='+encType+'&backtowifisetting1';	
	}
	else
	{
		para='wifisetting1.asp'
	}

	parent.TINY.box.show({iframe:para,boxid:'frameless',width:700,height:510,fixed:false,maskopacity:40});
}

function freshlist()
{
	location.reload(true); 	
}

</script>
</head>

<form method="post" name="entconfig" id="entconfig" action="/boafrm/entconfig">
<body>
<div class="int">
	<h1><strong>基点设备列表</strong><b class="rb" Onclick="freshlist()">刷新</b><b class="rb" onClick="backtowifisettings(-1);"; >返回</b></h1>
   	<div id="wirelesslist" class="datalist" style="display:block">

   		<% draw_wlSiteSurveyTbl(); %>    	
						
</div>
</body>
</form>
</html>
