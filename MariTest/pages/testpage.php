<?
class TestPage extends ContentPage{
	
	function DisplayContent() {
		
		if(!isset($_GET['test']) || !isset($_GET['union'])) {
			$_SESSION['error'] = "Не указан номер теста или объединение";
			header('Location:?page=tests');
		}

		if(isset($_SESSION['test' . $_GET['test']]))  {
			$test = $_SESSION['test' . $_GET['test']];

			if(is_string($test)) {
				$_SESSION['error'] = $test;
				header('Location:?page=tests');

			}
		}
		//Проверка, отправлялась ли форма уже или это первый запуск
		if(isset($_POST['filled'])) {
			//Проверка, все ли поля заполнены
			foreach ($test['testQuestions'] as $question) {
				if (!isset($_POST[$question['id']])) {
					$_SESSION['error'] = "Не все ответы были заполнены $i";
					unset($_SESSION['test' . $_GET['test']]);
					header('Location:?page=testpage&test=' . $_GET['test'] . "&union=" . $_GET['union']);
				}
			}

			//Теперь мы знаем, что на все вопросы ответы есть, поэтому можно начинать сверять их и выставлять баллы
			$rightAnswers = 0;
			//Проверка среди тестовых вопросов
			foreach ($test['testQuestions'] as $question) {

				foreach($question['answers'] as $answer) {
					$isError = 1;
					
					if ($answer['is_true'] == 1 && $_POST[$question['id']] == $answer['text']) {
						$rightAnswers++;
						$isError = 0;
						break;
					}
				}
				if($isError) {
						echo "<span class='question-error'>Вы ошиблись в вопросе</span>: ", 
						$question['header'], "<br/>", $question['text'], "<br/>";
				}
			}
			//Проверка среди обычных вопросов.
			foreach ($test['fullQuestions'] as $question) {
				$isError = 1;
				foreach($question['answers'] as $answer) {
					
//					echo trim(mb_strtolower($_POST[$question['id']], 'UTF-8')), ' ',  trim(mb_strtolower($answer['text'], 'UTF-8')), "<br/>";
					if ($answer['is_true'] == 1 && mb_strtolower($_POST[$question['id']], 'UTF-8') == mb_strtolower($answer['text'], 'UTF-8')) {
						$rightAnswers++;
						$isError = 0;
						break;
					}
				}
				if($isError) {
					echo "<span class='question-error'>Вы ошиблись в вопросе:</span> ", 
					$question['header'], "<br/>", $question['text'], "<br/>";
				}
			}

			//забивание данных.
			$message = $this->dbControl->PassTest($_GET['test'], $rightAnswers, $_SESSION['userName']);
			if($message) {
				$_SESSION['error'] = $message;
				header('Location:?page=testpage&test=' . $_GET['test'] . "&union=" . $_GET['union']);
			}
			else {
				echo "Поздравляем. Вы прошли тест \"", $test['name'], "\"<br/>",
					 "Вы набрали $rightAnswers баллов из " , $test['num_of_test_questions']+$test['num_of_full_questions'];
			}
			unset($_SESSION['test' . $_GET['test']]);
		}
		//Если впервые запущена страница.
 		else {
 			$test = 'test' . $_GET['test'];
			$_SESSION[$test] = $this->dbControl->GetTestById($_GET['test']);
			$test = $_SESSION[$test];

			echo 	"<h3>", $test['name'], "</h3>",
					$test['info'],
					
					//Форма
						"<form action =\"?page=testpage&test=", $_GET['test'], "&union=", $_GET['union'], 
					"\" method=\"post\" autocomplete=\"off\" enctype=\"multipart/form-data\" >";

			//Отображение тестовых вопросов
			foreach ($test['testQuestions'] as $question) {
				echo 	"<h4>", $question['header'], "</h4>",
						$question['text'];
						if($question['picture'] != '')
							echo '<br/><img src="', $question['picture'], '" width="250px" />';
						echo "<br/>Варианты ответов: <br/>";

				//Считаем, сколько у нас правильных ответов:
				$right = 0; $deletedAnswers = array();
				foreach($question['answers'] as $answer) {
					if($answer['is_true']) $right++;
				}
				for($i = $right; $i > 1; $i--) {

					do {
						$deletedAnswer = rand(0, $right-1);
					} while(MyLib::IsNameInRange($deletedAnswer, $deletedAnswers));
					unset($question['answers'][$deletedAnswer]);
					$deletedAnswers[] = $deletedAnswers;
				}

				//Выводим ответы
				shuffle($question['answers']);
				foreach($question['answers'] as $answer) {
					echo '<input type="radio" name="', $question['id'], '" value="', $answer['text'], '" />',
					$answer['text'], "<br/>";
				}

			}

			//Отображение обычных вопросов
			foreach ($test['fullQuestions'] as $question) {
				echo 	"<h4>", $question['header'], "</h4>",
								$question['text'];
						if($question['picture'] != '')
							echo '<br/><img src="', $question['picture'], '" width="250px" />';
						echo '<br/><textarea name ="', $question['id'], '" /></textarea><br/>';
			}		

			echo ' <input name="filled" type="hidden" value="filled" />
				<input type="submit" value="Отправить" />',
			"</form>";
		}
	}

} new TestPage();
?>