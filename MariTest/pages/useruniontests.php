<? //Страница для учителя, чтоб просматривать, как ученик писал тесты этого объединения.
class UserUnionTests extends ContentPage {

	var $neededRights = array('create test');

	function DisplayContent () {
		$userTests = $this->dbControl->GetUserPassedTests($_GET['user'], $_GET['union'], $_GET['year']);
		$userInfo = $this->dbControl->GetUserInfo($_GET['user']);

		?>
		<h3>Список пройденных тестов</h3>
		<?
		echo 	"Пользователь: ", $userInfo['name'], ' ', $userInfo['middle_name'], ' ', $userInfo['last_name'],
				"<br/>Объдинение: ", $_GET['union'], "<br/>Год обучения: ", $_GET['year'], "<br/>";
			if(sizeof($userTests)) {
				?>
				<table class="testTable">
					<tr> 
						<th>Название теста</th>
						<th>Объединение</th>
						<th>Тема</th>
						<th>Кол-во вопросов</th>
						<th>Правильных</th>
						<th>Дата прохождения</th>
					</tr>
			
				<?
					foreach(array_reverse($userTests) as $test) {
						echo "<tr>",
								"<td>", $test['name'], "</td>",
								"<td>", $test['unionName'], "</td>",
								"<td>", $test['theme'], "</td>",
								"<td>", $test['questionsNum'], "</td>",
								"<td>", $test['right_answers'], "</td>",
								"<td>", $test['date_passed'], "</td>",
							"</tr>";
					}
				?>
				</table>
				<?
			}
			else {
				echo "Пользователь не проходил никаких тестов";
			}

	}
} new UserUnionTests();

?>