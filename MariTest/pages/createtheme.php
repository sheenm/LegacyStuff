<?
class CreateTheme extends ContentPage {

	var $neededRights = array('create test', 'create question', 'create theme');

	function DisplayContent() {

		if(!isset($_GET['union']) || !isset($_GET['year'])) {
			$_SESSION['error'] = "Страницы не существует. :(";
			header('Location:?page=unions');
		}
		if(isset($_POST['name'])) {
			$message = $this->dbControl->CreateTheme($_POST['name'], $_POST['info'], $_GET['union'], $_GET['year']);
			if($message) {
				$_SESSION['error'] = $message;
				header('Location:?page=createtheme&union=' . $_GET['union'] . "&year=". $_GET['year']);
			} 
			else {
				$_SESSION['message'] = 'Тема добавена успешно';
				header('Location:?page=themes&union=' . $_GET['union'] . "&year=". $_GET['year']);
			}
		}
		?>
		<form action =<?="\"?page=createtheme&union=". $_GET['union']. "&year=" . $_GET['year'] . "\""?> 
			method="post" autocomplete="off" name='createThemeForm' enctype="mutipart/form-data">
			<span id='errorBox' class='error'></span>
		<table>
			<tr><td colspan="2"><h3>Добавить тему</h3></td><td></td></tr>
			<tr>
				<td>Название</td>
				<td><input type="text" name="name" /></td>
			</tr>
			<tr>
				<td>Описание</td>
				<td><textarea name="info"></textarea></td>
			</tr>
			<tr>
				<td></td>
				<td><input type="submit" value="Добавить тему" /></td>
			</tr>
		</table>
		</form>

		<script type="text/javascript">
			document.createThemeForm.onsubmit = function () {
				if(	document.createThemeForm.name.value = '' ||
					document.createThemeForm.info.value = '' ||)
				{
					document.getElementById('errorBox').innerHTML = 'Заполните все поля';
					return false;
				}
				else return true;
			}
		</script>
		<?

	}

}	new CreateTheme();

?>