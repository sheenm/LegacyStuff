<? //Страница создания объединения
class CreateUnionPage extends ContentPage {
	var $neededRights = array('create union');

	function DisplayContent() {
	if(isset($_POST['name']) && isset($_POST['info'])) {

		$message = $this->dbControl->CreateUnion($_POST['name'], $_POST['info']);
	  	if($message) {
	  		$_SESSION['error'] = $message;
	  	} else $_SESSION['message'] = "Объединение добавлено.";
		header('Location:?page=createunion');

	}
	?>
		<table>
			<form action ="?page=createunion" method="post" name='createUnionForm' autocomplete="off" enctype="mutipart/form-data">
				<span id='errorBox' class='error'></span>
				<tr><td colspan="2"><h3>Добавить объединение</h3></td><td></td></tr>
				<tr>
					<td>Название объединения</td>
					<td>
						<input type="text" name="name"/>
					</td>
				</tr>
				<tr>
					<td>Информация</td>
					<td>
						<textarea name="info"></textarea>
					</td>
				</tr>
				<tr>
					<td></td>
					<td><input type="submit" value='Добавить'/>
				</tr>
			</form>
		</table>
		<script type="text/javascript">
			document.createUnionForm.onsubmit = function () {
				if (document.createUnionForm.name.value == '' ||
					document.createUnionForm.info.value == '') {
					document.getElementById('errorBox').innerHTML = 'Заполните все поля';
					return false;
				}
				else return true; 
			}
		</script>
	<?
	}

}	new CreateUnionPage();

?>