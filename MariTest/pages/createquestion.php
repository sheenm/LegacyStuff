<? //Страница создания вопросов
class QuestionPage extends ContentPage {
	var $neededRights = array('create question');

	function DisplayContent() {
		
		if(isset($_POST['header'])) {
			//Загрузка картинки
			if(is_uploaded_file($_FILES["picture"]["tmp_name"]))
		  	{
			     // Если файл загружен успешно, перемещаем его
			     // из временной директории в конечную
			  	 $questionId = $this->dbControl->GetQuestionsLastId() + 1;
			  	 $pathToFile = "img/uploads/".  $questionId . substr($_FILES["picture"]["name"], strpos($_FILES["picture"]["name"], '.'));
			     move_uploaded_file($_FILES["picture"]["tmp_name"], $pathToFile);
		   	} else {
		    	$pathToFile = '';
		   	}

			//Превращение данных по ответам в нормальный, структуризированный массив
			$answers = array();
			$rightAnswersCount = 0;
			for($i = 1; $i <= $_POST['numberOfAnswers']; $i++) {
				
				if(isset($_POST['isRightAnswer'.$i])) {
					if($_POST['isRightAnswer'.$i] == "on") $_POST['isRightAnswer'.$i] = 1;
					else $_POST['isRightAnswer'.$i] = 0;
				}
				else $_POST['isRightAnswer'.$i] = 0;
				if($_POST['needFullAnswer'] == 1) $_POST['isRightAnswer'.$i] = 1;

				$rightAnswersCount+=$_POST['isRightAnswer'.$i];

				$answers[] = array(
					'text' => $_POST['answer'.$i], 
					'isTrue' => $_POST['isRightAnswer'.$i]
				);

			}
			
			if(($rightAnswersCount == 0 || $rightAnswersCount == count($answers)) && $_POST['needFullAnswer'] == 0) {
				$_SESSION['error'] = "Нет правильных или неправильных ответов в вопросе. Вопрос не добавлен";
				header('Location:?page=createquestion&union=' . $_GET['union'] . "&year=". $_GET['year']);
			}			
			//end Превращение данных по ответам в нормальный, структуризированный массив
			else {
				$query = $this->dbControl->AddQuestion($_POST['header'], $_POST['text'], $pathToFile, $_POST['needFullAnswer'], 
														$_POST['theme'], $_GET['union'], $_GET['year'], $answers);
				if($query) {
					$_SESSION['error'] = $query;
					header('Location:?page=createquestion&union=' . $_GET['union'] . "&year=". $_GET['year']);
				} else {
					echo "Вопрос добавлен";
					return;
				}
			}
		} 
	?>
		<form action =<?="\"?page=createquestion&union=",$_GET['union'], "&year=", $_GET['year'],
		"\"";?> method="post" name="addQuestionForm" autocomplete="off" enctype="multipart/form-data">
			<span id='errorBox' class='error'></span>
			<table>
				<tr><td colspan="2"><h3>Добавить вопрос</h3></td><td></td></tr>
				<tr>
					<td>Заголовок</td>
					<td><input type="text" name="header" /></td>
				</tr>
				<tr>
					<td>Текст вопроса</td>
					<td><textarea name="text" ></textarea></td>
				</tr>
				<tr>
					<td>Картинка</td>
					<td><input type="file" name="picture"/></td>
				</tr>
				<tr>
					<td>Тип вопроса</td>
					<td>
						<input type="radio" name="needFullAnswer" checked="true" value="0" /> Тестовый <br/>
						<input type="radio" name="needFullAnswer" value="1" /> С полным ответом
					</td>
				</tr>
				<tr>
					<td>Тема</td>
					<td>
						<select name="theme">
						<?
							if(isset($_GET['theme'])) $themeName = $_GET['theme'];
							else $themeName = '';

							if(isset($_GET['union']) && isset($_GET['year']))
							foreach($this->dbControl->GetUnionThemes($_GET['union'], $_GET['year']) as $theme) {
								if($theme['name'] == $themeName)
									echo "<option selected>", $theme['name'], "</option>";
								else echo "<option>", $theme['name'], "</option>";
							}
						?>
						</select>
					</td>
				</tr>
				<tr>
					<td>Ответы</td>
					<td id ="answersSection"></td>
				</tr>
				<tr>
					<td><input type="text" name="numberOfAnswers" hidden /></td>
					<td><input type="button" name="addAnswer" value="Добавить ответ" /><br/>
						<input type="submit" value="Добавить вопрос" />
					</td>
				</tr>
			</table>
		</form>
		
		<noscript>
			Включите JavaScript для возможности добавлять ответы к вопросу.
		</noscript>
		<script>
			var answersNumber = 0;
			addAnswer();
			document.addQuestionForm.addAnswer.onclick = addAnswer;

			document.addQuestionForm.needFullAnswer[0].onclick = 
			document.addQuestionForm.needFullAnswer[1].onclick = function() {
				answersNumber = 0;
				document.getElementById('answersSection').innerHTML = '';
				addAnswer();
			}

			function addAnswer() {
					answersNumber++;
					document.getElementById('answersSection').innerHTML += 
					"<input type='text' name='answer"+ answersNumber + "' />";
					if(document.addQuestionForm.needFullAnswer[0].checked) {
						document.getElementById('answersSection').innerHTML += 
							"<br/>Правильный? <input type='checkbox' checked='checked' name='isRightAnswer"+ answersNumber +"'><br/>";
					} else {
						document.getElementById('answersSection').innerHTML += "<br/>";
					}
					document.addQuestionForm.numberOfAnswers.value = answersNumber;

			}
			//проверка полей формы
			document.addQuestionForm.onsubmit = function() {
				if(document.addQuestionForm.header.value == '' || 
					document.addQuestionForm.text.value == '' || 
					document.addQuestionForm.theme.value == ''
					)
				{
					document.getElementById('errorBox').innerHTML = 'Заполните все поля';
					return false;
				}
				else {
					return true;
				}
			}
		</script>
	<?
	}
}	new QuestionPage();
?> 