<?//Страница создания теста
class CreateTestPage extends ContentPage {

	var $neededRights = array('create test');

	function DisplayContent() {

		if(isset($_POST['name'])) {

			$query = $this->dbControl->CreateTest($_POST['name'], $_GET['union'], $_GET['year'], $_POST['theme'], $_POST['info'], $_POST['testQuestionsNum'],
				$_POST['fullQuestionsNum']);
			if($query) {
				$_SESSION['error'] = $query;
				header('Location:?page=createtest&union=' . $_GET['union'] . "&year=". $_GET['year']);
			}
			else {
				echo 'Тест добавлен.';
				return;
			}
		}

		?>
		<form action =<?="\"?page=createtest&union=",$_GET['union'], "&year=", $_GET['year'],"\"";?> 
			method="post" name="createTestForm" autocomplete="off" enctype="mutipart/form-data">
		<span id='errorBox' class='error'></span>
		<table>
			<tr><td colspan="2"><h3>Добавить тест</h3></td></tr>
			<tr>
				<td>Название теста</td>
				<td><input type="text" name="name" /></td>
			</tr>
			<tr>
				<td>Тема</td>
				<td>
					<select name="theme">
						<?

							if(isset($_GET['union']) && isset($_GET['year']))
							foreach($this->dbControl->GetUnionThemes($_GET['union'], $_GET['year']) as $theme) {
								echo '<option>', $theme['name'], '</option>';
							}
						?>
					</select>
				</td>
			</tr>
			<tr>
				<td>Информация</td>
				<td><textarea name="info"></textarea></td>
			</tr>
			<tr>
				<td>Количество тестовых вопросов</td>
				<td><input type="text" name="testQuestionsNum" /></td>
			</tr>
			<tr>
				<td>Количество вопросов с ответом</td>
				<td><input type="text" name="fullQuestionsNum" /></td>
			</tr>
			<tr>
				<td></td>
				<td>
					<input type="submit" value="Создать тест" />
				</td>
			</tr>
		</table>
		</form>

		<script type="text/javascript">
			document.createTestForm.onsubmit = function() {
				if( document.createTestForm.name.value == '' ||
					document.createTestForm.info.value == '' ||
					document.createTestForm.testQuestionsNum.value == '' ||
					document.createTestForm.fullQuestionsNum.value == '')
				{
					document.getElementById('errorBox').innerHTML = 'Заполните все поля';
					return false;
				}
				else return true;
			}
		</script>
		<?
	}
	
} new CreateTestPage();
?>