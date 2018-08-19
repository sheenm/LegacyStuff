<? //Страница, на которой будут отображаться тесты, доступные пользователю. 
class TestsPage extends ContentPage {

	function DisplayContent() {
		$tests = $this->dbControl->GetUserTests($_SESSION['userName']);
		if(!count($tests)) { 
			echo "Вам не доступен ни один тест.";
			return;
		}
		?>
		<table class="testTable">
			<tr> 
				<th>Название теста</th>
				<th>Кол-во вопросов</th>
				<th>Объединение</th>
				<th>Тема</th>
			</tr>
			
			<?
				foreach($tests as $test) {
					$numOfQuestions = $test['num_of_full_questions'] + $test['num_of_test_questions'];
					echo "<tr>",
							"<td>", "<a href=\"?page=testpage&test=", $test['id'] , "&union=", $test['union'] , "\">" , $test['name'] ,"</a></td>",
							"<td>", $numOfQuestions ,"</td>",
							"<td>", $test['union'] ,"</td>",
							"<td>", $test['theme'] ,"</td>",
						"</tr>";
				}
			?>
		</table>
		<?
	}
} new TestsPage();