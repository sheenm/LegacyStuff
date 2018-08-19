<?	//Выводит список вопросов
class QuestionsPage extends ContentPage {

	var $neededRights = array ('create question');

	function DisplayContent() {
		$union = $_GET['union'];
		$yearStarted = $_GET['year'];
		$theme = $_GET['theme'];
		$questions = $this->dbControl->GetAllQuestionsFromTheme($_GET['theme'], $_GET['union'], $_GET['year']);
		
		if(!count($questions['testQuestions']) && !count($questions['fullQuestions'])) {
			echo "Не создано ни одного вопроса.";
			return;
		}

		?>
			<h3>Вопросы по теме <?=$theme?> 
				(<a href="<?="?page=createquestion&amp;union=$union&amp;year=$yearStarted&amp;theme=$theme"?>">Добавить вопрос</a>)
			</h3>
			<table class="testTable">
				<tr> 
					<th>Заголовок</th>
					<th>Изображение</th>
					<th>Текст</th>
					<th>Правильные Ответы</th>
					<th>Неправильные ответы</th>
				</tr>

		<?

		//Вывод тестовых вопросов
		foreach($questions as $subQuestions) {
			foreach($subQuestions as $question) {
				echo 	'<tr>',
							'<td>', $question['header'], '</td>',
							'<td>';
								if($question['picture'] != '') 
									echo '<img src="', $question['picture'], '" width="200px" />';
								else echo 'Нет картинки.';
					   echo '</td>',
							'<td>', $question['text'] , '</td>',
							'<td>'; //Правильные ответы
								
								for($i = 0, $j = 1;$i < count($question['answers']); $i++) {
									if($question['answers'][$i]['is_true']) {
										echo "$j. ", $question['answers'][$i]['text'], '<br/>';
										$j++;
									}
								}
					   echo '</td>', //Неправильные ответы
							'<td>';
								for($i = 0, $j=1;$i < count($question['answers']); $i++) {
									if(!$question['answers'][$i]['is_true']) {
										echo "$j. ", $question['answers'][$i]['text'], '<br/>';
										$j++;
									}
								} 
					   echo '</td>',
						'</tr>';
			}
		}

		//Вывод обычных вопросов


		echo "</table>";
	}
} new QuestionsPage();
?>