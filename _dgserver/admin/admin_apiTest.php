<?php
include "header.php";
$stopCommand = true;
$version=$_GET["version"];
if(!$version)$version=2;
include "../command/cmd".$version."_help.php";
include "../command/cmd".$version.".php";


?>
 <script>
var categoryName = "";

var changeDis = function(){
	var obj = $(".custom-combobox-input");
	var divName = obj.val();
	log("on div"+divName);

	categoryName = divName;

	if($("div[name="+divName+"]").attr("value")!="ok")return;

	$("div").css("display","none");
	$("div[name="+divName+"]").css("display","block");

	var datatable = getDataTable("datatable");
	datatable.attr("dbWhere",'{"category":"'+divName+'","limit":20}');
	loadDataTable(datatable,"reload");
}

$(document).ready(function(){

	$('body').on('input','.custom-combobox-input',function(){
		//if(!$(this).hasClass('LQEditor'))return;

		changeDis();

	});


    $(".custom-combobox-input").change(function(){
 
        alert ("change event occured with value: " + document.getElementById("txtchange").value);
 
  });
 

	$('body').on('click','.doAction',function(){
		var api = $(".custom-combobox-input").val();
		var pdata = getLQEditorValue("p");
		var param = {"mode":"nodes","version":<?=$version?>,"gid":gid};
		var apiurl = "../data.php";

		param["a"]=j2s(api);
		param["p"]=j2s(pdata);



		$.ajax({
				    url : apiurl, 
				    data : param,
				    dataType : "json", 
				    type : "get",
				    success : function(data){

				    	var resultDisplay = $("td[name=resultDisplay]");
				    	//result = "요청:"+apiurl+"<br>파마메터:"+j2s(param)+"<br><br>결과:<br><textarea cols=130 rows=5>"+j2s(data["0"])+"</textarea>";
						result = "요청:"+apiurl+"<br>파마메터:"+j2s(param)+"<br><br>결과:<br><textarea cols=130 rows=5>"+j2s(data["0"])+"";
						result+="\n\n\n";

              if(typeof(data["0"]["log"]) == "object"){
				    	for(var i=0; i<data["0"]["log"].length;i++){
				    		result+=data["0"]["log"][i]+"\n\n";

				    	}
              }
				    	result+="</textarea>";


						resultDisplay.html(result);


						var datatable = getDataTable("datatable");
						datatable.attr("dbWhere",'{"category":"'+categoryName+'","limit":10}');
						loadDataTable(datatable,"reload");
					}
					,
					error: function(e) {
					    	alert("error",j2s(e));
					    }
					});



	});

});

</script>
  <script>
  (function( $ ) {
    $.widget( "custom.combobox", {
      _create: function() {
        this.wrapper = $( "<span>" )
          .addClass( "custom-combobox" )
          .insertAfter( this.element );
 
        this.element.hide();
        this._createAutocomplete();
        this._createShowAllButton();
      },
 
      _createAutocomplete: function() {
        var selected = this.element.children( ":selected" ),
          value = selected.val() ? selected.text() : "";
 
        this.input = $( "<input>" )
          .appendTo( this.wrapper )
          .val( value )
          .attr( "title", "" )
          .addClass( "custom-combobox-input form-control form-control-inline" )
          .autocomplete({
            delay: 0,
            minLength: 0,
            source: $.proxy( this, "_source" ),
          })
          .tooltip({
            tooltipClass: "ui-state-highlight"
          });
 
        this._on( this.input, {
        	autocompleteclose:function( event, ui ) {
        		changeDis();
        	},
          autocompleteselect: function( event, ui ) {
            ui.item.option.selected = true;
            this._trigger( "select", event, {
              item: ui.item.option
            });
            //changeDis();
          },
 
          autocompletechange: "_removeIfInvalid"
        });
      },
 
      _createShowAllButton: function() {
        var input = this.input,
          wasOpen = false;
 
        $( "<button>" )
          .attr( "tabIndex", -1 )
          .attr( "title", "Show All Items" )
          .tooltip()
          .appendTo( this.wrapper )
       	  .html("select")
          .removeClass( "ui-corner-all" )
          .addClass( "custom-combobox-toggle btn btn-primary" )
          .mousedown(function() {
            wasOpen = input.autocomplete( "widget" ).is( ":visible" );
          })
          .click(function() {
            input.focus();
            // Close if already visible
            if ( wasOpen ) {
              return;
            }
 
            // Pass empty string as value to search for, displaying all results
            input.autocomplete( "search", "" );
          });
      },
 
      _source: function( request, response ) {
        var matcher = new RegExp( $.ui.autocomplete.escapeRegex(request.term), "i" );
        response( this.element.children( "option" ).map(function() {
          var text = $( this ).text();
          if ( this.value && ( !request.term || matcher.test(text) ) )
            return {
              label: text,
              value: text,
              option: this
            };
        }) );
      },
 
      _removeIfInvalid: function( event, ui ) {
 
        // Selected an item, nothing to do
        if ( ui.item ) {
          return;
        }
 
        // Search for a match (case-insensitive)
        var value = this.input.val(),
          valueLowerCase = value.toLowerCase(),
          valid = false;
        this.element.children( "option" ).each(function() {
          if ( $( this ).text().toLowerCase() === valueLowerCase ) {
            this.selected = valid = true;
            return false;
          }
        });
 
        // Found a match, nothing to do
        if ( valid ) {
          return;
        }
 
        // Remove invalid value
        this.input
          .val( "" )
          .attr( "title", value + " didn't match any item" )
          .tooltip( "open" );
        this.element.val( "" );
        this._delay(function() {
          this.input.tooltip( "close" ).attr( "title", "" );
        }, 2500 );
        this.input.data( "ui-autocomplete" ).term = "";
      	//changeDis();
      },
 
      _destroy: function() {
        this.wrapper.remove();
        this.element.show();
      }
    });
  })( jQuery );
 
  $(function() {
    $( "#combobox" ).combobox();

  });

  </script>

<center>
현재버전 : <?=$version?> / 
<a href=admin_apiTest.php?version=2>graphdog ver2</a> / 
<a href=admin_apiTest.php?version=3>graphdog ver3</a>
<br>
 <table class="LQDataEditor" border=1 width=50%>
 	<tr>
 		<td colspan=2>
 			<?php




$class_methods = get_class_methods("commandHelpClass");
$apiList=array();

$apiList[]="-----select api-------";
$optionList="<option value=''></option>";
foreach ($class_methods as $method_name) {
    if(strpos($method_name,"help_")  !== false){
	    $realName = str_replace("help_","",$method_name);
	    $mInfo = commandHelpClass::$method_name();
	    $apiList[]=$realName;
	    $optionList.="<option value='$realName'>$realName</option>";
	    echo "<div name='".$realName."'' style='display:none' value='ok'>";
	    echo "<font size=6 color=blue>".$realName."</font><br>";
	    echo "<b>".$mInfo["description"]."</b><br><br>";
	    echo "* parameter <br>";
	    echo "<table border=1><tr><td>name</td><td>type</td><td>description</td></tr>";
	    for($i=0;$i<count($mInfo["param"]);$i++){
		    echo "<tr><td>".$mInfo["param"][$i]["name"]."</td><td>".$mInfo["param"][$i]["type"]."</td><td>".$mInfo["param"][$i]["description"]."</td></tr>";
	    }
	    echo "</table><br>";
	    
	    echo"* result<br>";
	    echo "<table border=1><tr><td>code</td><td>name</td><td>message</td></tr>";
	    for($i=0;$i<count($mInfo["result"]);$i++){
		    echo "<tr><td>".$mInfo["result"][$i]["code"]."</td><td>".$mInfo["result"][$i]["name"]."</td><td>".$mInfo["result"][$i]["message"]."</td></tr>";
	    }
	    echo "</table><br>";


	    if($mInfo["comment"]){
		    echo"* comment<br>";
		    echo "<table border=1><tr><td>".nl2br($mInfo["comment"])."</td></tr>";

		    echo "</table><br><br>";
		}

	    echo"</div>";
    }
}

$apiListStr = json_encode($apiList,JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK);
	
 			?>

 		</td>

 	</tr>
 	<tr>
		<td>api</td>	
		<td> 
		 <select id="combobox" class="LQEditor">
		 		<?php
		 			echo $optionList;
		 		?>
			  </select>
</td>
	</tr>
	<tr>
		<td>param</td>
		<td field="p" editor='{"type":"dictionary"}'></td>
	</tr>
	<tr><td colspan=2><input type="submit" value="실행" class="doAction"></td></tr>
	<tr><td colspan=2 name="resultDisplay">
		

	</td></tr>
</table>

<script>

</script> 
<br><br>
<h2 id="tables-contextual-classes">|최근 로그</h2>
<table class="LQDataTable" dbSource="dataManager2.php"  dbClass="UserLog" dbWhere='{"id":"*"}' dbSort='{"no":"desc"}' autoSetting="true" dbLimit="10" name="datatable" border=1>
	<thead>
	</thead>
	<tbody datazone>
	</tbody>
</table>

</center>
<?php
include "footer.php";
?>