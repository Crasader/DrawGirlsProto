				<br><br><br><br>
				<textarea rows=10 class="form-control">
					
					<?=json_encode(LogManager::getLogAndClear(),JSON_UNESCAPED_UNICODE | JSON_NUMERIC_CHECK)?>

				</textarea>

				</div> <!--col-lg-2-->
			</div><!--row-->
		</div><!--container-fluid-->

	</body>
</html>


<?php 

DBServer::closeAllConnection();

if($connect){
	@mysql_close();
} 
?>