<?
class AddUserUnion extends ContentPage{

	var $neededRights = array();

	function DisplayContent() {
		
		if(isset($_POST['union'])) {
			if(MyLib::IsNameInRange('create union', $_SESSION['rights']))
				$approved = 1;
			else $approved = 0;
			
			if($_POST['year'] != '')		
				$year = MyLib::AgesToYear($_POST['year'], 9);
			else $year= '';

			$message = $this->dbControl->CreateUnionForUser($_POST['union'], $_SESSION['userName'], $year, $approved);
			if($message) {
				$_SESSION['error'] = $message;
			}
			else {
				if ($approved) {
				$_SESSION['message'] = "Объединение добавлено";
				$_SESSION['unions'][] = array(
					'name' 	=> $_POST['union'], 
					'year_started' => $year
					);
				}
			}
			
			header('Location:?page=adduserunion');
		}

		?>
		<form action ="?page=adduserunion" method="post" name="adduserunionform" autocomplete="off" enctype="mutipart/form-data">
		<span id='errorBox' class='error'></span>
		<table>
			<tr>
				<td>Название</td>
				<td><select name="union" type="text" > 
					<?
						foreach($this->dbControl->GetUnionsList() as $union) {
							echo "<option>$union</option>";
						}
					?>
					</select>
				</td>
			</tr>
			<tr>
				<td>Год</td>
				<td><input type="number" name="year" /></td>
			</tr>
			<tr>
				<td></td>
				<td><input type="submit" /></td>
			</tr>
		</table>
		</form>
		<script type="text/javascript">
			document.adduserunionform.onsubmit = function() {
				if(document.adduserunionform.union.value != '' && document.adduserunionform.year.value != '') {
					return true;
				}
				else {
					document.getElementById('errorBox').innerHTML = 'Заполните все поля';
					return false;
				}
			}
		</script>
		<?
	}
} new AddUserUnion();
?>