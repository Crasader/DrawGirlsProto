<?php
include "manage_header.php";
?>
<script>

$(document).ready(function(){

	$('body').on('click','#isPre',function(){
		$('button[id=isPre]').each(function(index){
			$(this).removeClass("active");
		});
		$(this).addClass("active");
		
		var dataview = getDataTable("datatable");
		var selectType = $('.active[id=isPre]').val();
		dataview.attr("dbWhere",'{"isPre":"'+selectType+'"}');
		loadDataTable(dataview);
	});
});

var gachaLevelViewer = function(value,option){
	
	if(value==2){
		return "<b><font color=green>2</font></b>";
	}

	if(value==3){
		return "<b><font color=blue>3</font></b>";
	}

	if(value==4){
		return "<b><font color=red>4</font></b>";
	}


	if(value==1){
		return "<b><font color=black>1</font></b>";
	}

	return value;
	//return value.substr(0,4)+"-"+value.substr(4,4)+"-"+value.substr(8,4);
}

</script>

<table width=100%><tr><td>
	<h2 id="tables-contextual-classes">|뽑기구성</h2>
</td><td align=right>
	<a class='LQSaveCSVBtn' tableName='datatable' download="filename.csv" href="#">CSV 다운받기</a>
</td></tr></table>
<div class="btn-group radio-btn" data-toggle-name="radius_options" data-toggle="buttons-radio">	
	<button id="isPre" type="button" value="*" class="btn btn-default active" data-toggle="button">모두보기</button>
	<button id="isPre" type="button" value="true" class="btn btn-default" data-toggle="button">고급</button>
	<button id="isPre" type="button" value="false" class="btn btn-default" data-toggle="button">일반</button>
</div>
<table class="LQDataTable" dbSource="dataManager2.php" dbClass="Gacha" dbWhere='{}' dbSort='{"level":"asc"}' dbLimit="100" name="datatable" border=1 align=center>
	<thead>
		<tr>
		<th primary field='no' viewer='{"type":"text"}' >no</th>
		<th field='exchangeID' viewer='{"type":"exchangeviewer"}' editor='{"type":"exchangemaker","content":"뽑기","statsID":"gacha","statsValueField":"no"}' >교환ID</th>
		<th field='level' viewer='{"type":"gachaLevelViewer"}' editor='{"type":"select","element":[1,2,3,4]}' >level</th>
		<th field='premiumOnly' viewer='{"type":"select","element":["고급뽑기","일반뽑기"],"value":[1,0]}' editor='{"type":"select","element":["고급뽑기","일반뽑기"],"value":[1,0]}' >고급전용</th>
		<th field='showProb' viewer='{"type":"text"}' editor='{"type":"text"}' >노출확률<br>(0~100)</th>
		<th field='gainProb' viewer='{"type":"text"}' editor='{"type":"text"}' >획득확률<br>(1~100)</th>
		<th manage='delete update insert' ></th>
		</tr> 
	</thead>
	<tbody datazone>

</tbody>
</table>



<?php
include "manage_footer.php";
?>