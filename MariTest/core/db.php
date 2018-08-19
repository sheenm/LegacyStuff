<?	//Доступ к базе данных осуществляется через этот файл.
class DB {
	var $mysqlConnect;
	var $teacherCode = "ILoveMaria";	
	function DB(){
		$this->DBConnect();		
	}
	//Позволяет подсоединиться к БД.
	function DBConnect() {
		$host="localhost";
		$db="MariTest";
		$user="Mari";
		$password="Mari";

		$this->mysqlConnect = mysqli_connect($host,$user,$password) or
		 trigger_error(mysql_error(), E_USER_ERROR);

		$selectDB = mysqli_select_db($this->mysqlConnect, $db);
	}

	/** 	Функции связанные с пользователями.			
	**/
	function CheckUserPassword($user, $password)
	{
		$user = addslashes($user);
		$password = addslashes($password);

		$password =  md5($password);
		$query = "SELECT * from users
				  WHERE users.login=\"$user\"
				  AND users.password=\"$password\"";
		$result = mysqli_query($this->mysqlConnect,$query);
		return mysqli_num_rows($result);
	}

	function GetUserID($user) {
		//$user = addslashes($user);

		$userID = "SELECT id FROM users
				WHERE users.login =\"$user\"";
		$userID = mysqli_query($this->mysqlConnect,$userID);
		if(!mysqli_num_rows($userID)) return 0;
		$userID = mysqli_fetch_array($userID);
		$userID = $userID[0];

		return $userID;
	}

	function RegisterUser($userData) {

		if($this->GetUserID($userData['login'])) {
			return "Такой пользователь уже существует.";
		}
		if(trim($userData['login']) == '') {
			return "Введите имя пользователя";
		}
		if ($userData['password'] != $userData['passwordRepeat']) {
			return "Пароли не совпадают. Повторите попытку.";
		}
		if(trim($userData['password']) == '') {
			return "Введите пароль";
		}
		if(trim($userData['name']) == '') {
			return "Введите своё имя";
		}
		if(trim($userData['middleName']) == '') {
			return "Введите отчество";
		}
		if(trim($userData['lastName']) == '') {
			return "Введите фамилию";
		}
		if(sizeof($userData['unions']) == 0) {
			return "Введите данные по объединению";
		}

		$login = addslashes($userData['login']);
		$password = md5(addslashes($userData['password']));
		$name = addslashes($userData['name']);
		$middleName = addslashes($userData['middleName']);
		$lastName = addslashes($userData['lastName']);

		$query ="INSERT INTO users(id,login,password,name,middle_name,last_name)
				VALUES ('',\"$login\",\"$password\",\"$name\",\"$middleName\",\"$lastName\")";
		mysqli_query($this->mysqlConnect,$query);
		
		//Юзер сделан. Необходимо теперь присвоить ему права.
		if($userData['code'] == $this->teacherCode) {
			$userData['rights'] = array('create test', 'create question', 'create union', 'approve user', 'create theme');
			$unionsAreApproved = 1;
		}
		else {
			$userData['rights'] = array();
			$unionsAreApproved = 0;
		}

		foreach ($userData['rights'] as $right) {
			$this->CreateRightForUser($right, $userData['login']);
		}

		//Объединения. 
		foreach ($userData['unions'] as $union => $yearStarted) {
			$this->CreateUnionForUser($union, $userData['login'], $yearStarted, $unionsAreApproved);
		}

		return "Пользователь зарегистрирован успешно";
	}

	function CreateRight($right, $info) {
		$query = "SELECT * FROM  rights
				WHERE rights.right =\"$right\"";
		if (mysqli_num_rows(mysqli_query($this->mysqlConnect,$query))) return 1;

		$query = "INSERT INTO rights (`id`, `right`, `info`) 
					VALUES ('','$right', '$info')";
		mysqli_query($this->mysqlConnect,$query);
		return 0;
	}

	function CreateRightForUser($right, $user) {
		$rightID = "SELECT id FROM rights
				WHERE rights.right=\"$right\"";
		$rightID = mysqli_query($this->mysqlConnect,$rightID);
		if(!mysqli_num_rows($rightID)) return 1;
		$rightID = mysqli_fetch_array($rightID);
		$rightID = $rightID[0];

		$userID = $this->GetUserID($user);
		if(!$userID) return 1;

		$query = "SELECT * FROM users_rights
				WHERE users_rights.id_user=$userID
				AND users_rights.id_right=$rightID";
		if (mysqli_num_rows(mysqli_query($this->mysqlConnect,$query))) return 1;

		$query = "INSERT INTO users_rights(id,id_user,id_right)
				VALUES ('', $userID, $rightID)";
		mysqli_query($this->mysqlConnect,$query);
		return 0;
	}

	function CreateUnion($name, $info) {
		if(trim($name) =='' || trim($info)=='') return "Не все данные заполнены.";

		$name = addslashes($name);
		$info = addslashes($info);

		$query = "SELECT * FROM unions
				WHERE unions.name=\"$name\"";
		if (mysqli_num_rows(mysqli_query($this->mysqlConnect,$query))) return "Объединение уже существует.";

		$query = "INSERT INTO unions (`id`, `name`, `info`) 
					VALUES ('','$name', '$info')";
		mysqli_query($this->mysqlConnect,$query);
		return 0;
	}

	function CreateUnionForUser($union, $user, $yearStarted, $isApproved) {
		if(trim($union) =='' || trim($yearStarted) =='') return "Не все данные заполнены";

		$union = addslashes($union);
		$user = addslashes($user);

		$unionID = $this->GetUnionID($union);
		$userID = $this->GetUserID($user);
		if(!$userID) return 'Пользователь не найден';

		$query = "SELECT * FROM users_unions
				WHERE users_unions.id_user=$userID
				AND users_unions.id_union=$unionID
				AND users_unions.year_started=$yearStarted";
		if (mysqli_num_rows(mysqli_query($this->mysqlConnect,$query))) {
			
			return 'Вы уже находитесь в данном объединении';
		}

		$query = "INSERT INTO users_unions(id,id_user,id_union,year_started,is_approved)
				VALUES ('', $userID, $unionID,$yearStarted, $isApproved)";
		mysqli_query($this->mysqlConnect,$query);

		return 0;
	}
	function GetUserRights($user) {
		$user = addslashes($user);

		$userID = $this->GetUserID($user);
		if(!$userID) return array();

		$query = "SELECT rights.right
			FROM rights
			RIGHT JOIN users_rights
				ON users_rights.id_right = rights.id
			RIGHT JOIN users
				ON users.id = users_rights.id_user
			WHERE users.id=$userID";

		$result = mysqli_query($this->mysqlConnect,$query);
		return MyLib::MySqlQueryToArray($result);

	}
	function GetUserUnions($user) {
		$user = addslashes($user);
		$userID = $this->GetUserID($user);
		if(!$userID) return array();

		$query = "SELECT unions.name, users_unions.year_started
			FROM unions
			RIGHT JOIN users_unions
				ON users_unions.id_union = unions.id
			RIGHT JOIN users
				ON users.id = users_unions.id_user
			WHERE users.id=$userID
				AND users_unions.is_approved=1";

		$result = mysqli_query($this->mysqlConnect,$query);
		return MyLib::MySqlQueryToArray($result, 1);
	}

	function GetUserTests($user) {
		$user = addslashes($user);
		$userID = $this->GetUserID($user);
		if(!$userID) return array();

		$query = "	SELECT  tests.id, tests.name, tests.num_of_test_questions, tests.num_of_full_questions,
							themes.name as \"theme\", unions.name as \"union\"		
					FROM tests 
					INNER JOIN themes 
						ON themes.id=tests.id_theme 
					INNER JOIN unions 
						ON unions.id=themes.id_union 
					INNER JOIN users_unions 
						ON users_unions.id_union=unions.id 
					INNER JOIN users 
						ON users.id=users_unions.id_user 
					WHERE users.id=$userID
						AND users_unions.is_approved=1
						AND users_unions.year_started=
						IF (MONTH(NOW()) < 9, YEAR(NOW()) - themes.union_year, YEAR(NOW()) - themes.union_year +1)";

		$result = mysqli_query($this->mysqlConnect,$query);
		return MyLib::MySqlQueryToArray($result, 1);
	}

	function GetTestById($id) {
		$query = "	SELECT tests.name, unions.name as unionName, tests.id_theme, tests.info, 
					tests.num_of_test_questions, tests.num_of_full_questions 
					FROM tests 
					INNER JOIN  themes 
						ON tests.id_theme=themes.id 
					INNER JOIN unions 
						ON unions.id=themes.id_union 
					WHERE tests.id=$id";
		$result = mysqli_query($this->mysqlConnect,$query);
		$result = MyLib::MySqlQueryToArray($result, true);
		if(!isset($result[0])) return "Не удалось загрузить тест";
		$result = $result[0]; //т.к. результат только один.

		//Записывание вопросов для теста.
		$result['testQuestions'] = $this->GetQuestions($result['id_theme'], 'test', $result['num_of_test_questions']);
		if(is_string($result['testQuestions'])) return $result['testQuestions'];
		$result['fullQuestions'] = $this->GetQuestions($result['id_theme'], 'full', $result['num_of_full_questions']);
		if(is_string($result['fullQuestions'])) return $result['fullQuestions'];
		return $result;
	}

	function GetQuestionAnswers($questionID) {
		$answers = "SELECT answers.text, answers.is_true FROM  answers 
					WHERE answers.id_question=$questionID
					ORDER BY answers.is_true DESC";
		$answers = mysqli_query($this->mysqlConnect, $answers);
		return MyLib::MySqlQueryToArray($answers, 1);
	}

	function GetQuestions($themeID, $type, $neededNumber) {
		if($type == 'test') $type = 0;
		else $type = 1;

		$questionsNum = $this->GetQuestionsNumber($themeID, $type);

		if($neededNumber > $questionsNum) return "В базе данных не нашлось столько вопросов";
		else if($neededNumber == $questionsNum) {
			$questions = "	SELECT questions.header, questions.id, questions.text, questions.picture 
							FROM questions  
							WHERE  questions.need_full_answer=$type AND questions.id_theme=$themeID";
			$questions = mysqli_query($this->mysqlConnect, $questions);
			$questions = MyLib::MySqlQueryToArray($questions, 1);
			for($i = 0; $i < count($questions); $i++) {
				$questions[$i]['answers'] = $this->GetQuestionAnswers($questions[$i]['id']);
			}
		}
		else {
				$questions = array(); $rowTaken = array();
				for($i = 0; $i < $neededNumber; $i++) {
		
					do {
						$currentRow = rand(0, $questionsNum-1);
		
					} while (MyLib::IsNameInRange($currentRow,$rowTaken));
					$rowTaken[] = $currentRow;
		
					$question = "	SELECT questions.header, questions.id, questions.text, questions.picture 
									FROM questions  
									WHERE  questions.need_full_answer=$type AND questions.id_theme=$themeID
									LIMIT $currentRow,	1";
					$question = mysqli_query($this->mysqlConnect, $question);
					$question = MyLib::MySqlQueryToArray($question, 1);
					$question = $question[0];
					//Каждому вопросу надо выбрать ответы.
					$question['answers'] = $this->GetQuestionAnswers($question['id']);
					$questions[] = $question;
				}
		}
		return $questions;
	}

	/** 	Конец функций связанные с пользователями.			
	**/
	
	/*	Для утверждения учителем списка объединений*/
	function GetUnionUsers($union, $ages, $approved) { //относительный год
		$union = addslashes($union);
		$year = MyLib::AgesToYear($ages, 9);
		$query = "SELECT users.* FROM users
				INNER JOIN users_unions
					ON users_unions.id_user=users.id
				INNER JOIN unions
					ON unions.id=users_unions.id_union
				WHERE users_unions.is_approved=$approved
					AND unions.name=\"$union\"
					AND users_unions.year_started=$year";
		$result = mysqli_query($this->mysqlConnect,$query);
		return MyLib::MySqlQueryToArray($result, true);

	}

	function GetUserInfo($name) {
		$name = addslashes($name);
		$query ="SELECT * FROM users WHERE users.login=\"$name\"";
		
		$result = mysqli_query($this->mysqlConnect,$query);
		if($result) $numResults = mysqli_num_rows ($result);
		
		if(!$numResults) return array();
		else {
			return mysqli_fetch_array($result);
		}
	}

	function GetUserPassedTests($user, $union = 0, $yearStarted = 0) {
		$user = addslashes($user);
		$userID = $this->GetUserId($user);
		if(!$userID) return "Пользователь не найден";

		$query = "  SELECT users_tests.right_answers, tests.name, users_tests.date_passed,
						   tests.num_of_full_questions+tests.num_of_test_questions as questionsNum,
						   tests.info, themes.name as theme, unions.name as unionName
					FROM users_tests
					INNER JOIN tests
						ON tests.id=users_tests.id_test
					INNER JOIN themes
						ON themes.id=tests.id_theme
					INNER JOIN unions
						ON unions.id=themes.id_union
					WHERE users_tests.id_user=$userID";
		if($union !== 0) {

			$union = addslashes($union);	
			$unionID = $this->GetUnionID($union);
			//$yearStarted = MyLib::AgesToYear($yearStarted);
			$query .= " AND themes.union_year=$yearStarted
						AND unions.id =$unionID";
		}

		$result = mysqli_query($this->mysqlConnect,$query);
		return MyLib::MySqlQueryToArray($result, true);
	}

	function PassTest($testID, $rightAnswers, $user) {
		$user = addslashes($user);
		$userID = $this->GetUserId($user);
		if(!$userID) return "Пользователь не найден. Тест не пройден";

		$query = "	INSERT INTO users_tests(id,id_user,id_test,right_answers,date_passed)
					VALUES ('',$userID,$testID,$rightAnswers, DATE_FORMAT(NOW(), '%d.%m.%Y'))";
		if(!mysqli_query($this->mysqlConnect,$query)) return "Внутренняя ошибка: тест не пройден";
		return 0;
	}

	function ApproveUsers($union, $year, $users, $approved=1) {
		$union = addslashes($union);
		$unionID = $this->GetUnionID($union);
		$year = MyLib::AgesToYear($year);

		$query = "UPDATE users_unions
			INNER JOIN users
				ON users.id=users_unions.id_user
			SET users_unions.is_approved=$approved
			WHERE users_unions.id_union=$unionID
				AND users_unions.year_started=$year
				AND users.login in (";
		
		foreach ($users as $user) {
			$user = addslashes($user);
			$query .= '"' . $user . '",';
		}
		$query = trim($query, ',');
		$query .= ')';

		if(!mysqli_query($this->mysqlConnect,$query)) return "Не удалось выполнить операцию" . count($users);
		else return 0;
	}
	/*end	Для утверждения учителем списка объединений*/
	function GetUnionsList() {
		$query = "SELECT name FROM unions";
		$result = mysqli_query($this->mysqlConnect,$query);
		return MyLib::MySqlQueryToArray($result);
	}

	function GetUnionThemes($union, $year) { //Здесь имеется в виду отностельный год.
		$union = addslashes($union);

		$query = "SELECT themes.name, themes.info from themes
				INNER JOIN unions
					ON unions.id=themes.id_union
				WHERE unions.name=\"$union\"
					AND themes.union_year=$year";
		$result = mysqli_query($this->mysqlConnect,$query);
		return MyLib::MySqlQueryToArray($result, true);
	}

	function GetThemeId($name, $union, $year) {
		//$union = addslashes($union);
		//$name = addslashes($name);

		$unionID = $this->GetUnionID($union);

		$themeID = "	SELECT id FROM themes
						WHERE themes.name=\"$name\"
							AND themes.id_union=$unionID
							AND  themes.union_year=$year";
		$themeID = mysqli_query($this->mysqlConnect,$themeID); 
		if(!mysqli_num_rows($themeID)) return 0;
		$themeID = mysqli_fetch_array($themeID);
		$themeID = $themeID[0];

		return $themeID;

	}

	function GetQuestionId($text) {
		$questionID = "SELECT id from questions WHERE questions.text=\"$text\"";
		$questionID = mysqli_query($this->mysqlConnect,$questionID); 
		if(!mysqli_num_rows($questionID)) return 0;
		$questionID = mysqli_fetch_array($questionID);
		$questionID = $questionID[0];

		return $questionID;		
	}

	function GetUnionID ($union) {
		
		$unionID = "SELECT id FROM unions
				WHERE unions.name=\"$union\"";
		$unionID = mysqli_query($this->mysqlConnect,$unionID);
		if(!mysqli_num_rows($unionID)) return 1;
		$unionID = mysqli_fetch_array($unionID);
		$unionID = $unionID[0];
		return $unionID;
	}

	function GetAllQuestionsFromTheme($theme, $union, $yearUnion) {
		$union = addslashes($union);
		$theme = addslashes($theme);
		$themeID = $this->GetThemeId($theme, $union, $yearUnion);
		$testQuestionsNum = $this->GetQuestionsNumber($themeID, 0);
		$fullQuestionsNum = $this->GetQuestionsNumber($themeID, 1);

		$questions = array(
			'testQuestions' => $this->GetQuestions($themeID, 'test', $testQuestionsNum),
			'fullQuestions' => $this->GetQuestions($themeID, 'full', $fullQuestionsNum)
		);
		return $questions;
	}

	function GetQuestionsNumber($themeID='', $type='') {
		$query = "SELECT count(id) FROM questions ";

		if($type !== '') {
			$query .= " WHERE questions.id_theme=$themeID AND questions.need_full_answer=$type";
		}
		$result = mysqli_query($this->mysqlConnect,$query);
		$result = mysqli_fetch_array($result);
		return $result[0];
	}

	function GetQuestionsLastId() {
		$result = mysqli_query($this->mysqlConnect, "SELECT max(id) FROM  questions");
		$result = mysqli_fetch_array($result);
		return $result[0];
	}
	/*
		Функции добавления контента
	*/
	function AddQuestion($header, $text, $picture, $needFullAnswer, $theme, $union, $year, $answers) {
		if(trim($header)=='' || trim($text)=='' || trim($union)=='') {
			return "Не заполнены все поля";
		}

		$header = addslashes(MyLib::EscapeHTMLTags($header));
		$text = addslashes(MyLib::EscapeHTMLTags($text));

		$themeID = $this->GetThemeId($theme, $union, $year);
		if(!$themeID) return "Не существует данной темы ($theme)";

		$query = "INSERT INTO questions(id,header,text,picture,need_full_answer,id_theme)
				VALUES ('',\"$header\",\"$text\",\"$picture\",$needFullAnswer,$themeID)";
		
		if(!mysqli_query($this->mysqlConnect,$query))
			return "Не удалось добавить вопрос. ";

		$questionID = $this->GetQuestionId($text);
		foreach ($answers as $answer) {
			$answerText =  addslashes(trim(MyLib::EscapeHTMLTags($answer['text'])));
			$answerIsTrue = $answer['isTrue'];
			$query = "INSERT INTO answers(id,id_question,text,is_true)
				VALUES ('', $questionID, \"$answerText\", $answerIsTrue)";
			if(!mysqli_query($this->mysqlConnect,$query)) return "Не все ответы были добавлены ($answerText).";
		}
		return 0;
	}

	function CreateTheme($name, $info, $union, $year) {
		$name = addslashes($name);
		$union = addslashes($union);

		if(trim($name)=='' || trim($info) == '') {
			return "Не заполнены все поля";
		}

		$unionID = $this->GetUnionID($union);
		if(!$unionID) return "Объединение не найдено";

		if($this->GetThemeId($name, $union, $year)) return "Тема уже существует";

		$query = "INSERT INTO themes(id,name,info,id_union,union_year)
			VALUES('',\"$name\",\"$info\",$unionID, $year)";
		if(!mysqli_query($this->mysqlConnect,$query)) return "Тема не добавлена.";
		return 0;
	}

	function DeleteTheme($union, $yearUnion, $name) {
		$union = addslashes($union);
		$name = addslashes($name);
		$themeID = $this->GetThemeId($name, $union, $yearUnion);

		//Удаление темы
		$query = "	DELETE FROM themes
					WHERE id=$themeID";
		if(!mysqli_query($this->mysqlConnect,$query)) return "Не удалось удалить тему";

		//Удаление тестов, связанных с этой темой.
		$query = "DELETE FROM tests
					WHERE id_theme=$themeID";
		if(!mysqli_query($this->mysqlConnect,$query)) return "Не удалось удалить тесты темы";

		//Удаление вопросов, связанных с темой.
		$query = "SELECT questions.id FROM questions
					WHERE id_theme=$themeID";
		$result = mysqli_query($this->mysqlConnect,$query);

		foreach(MyLib::MySqlQueryToArray($result) as $questionID) {
			$query = "DELETE FROM answers WHERE id_question=$questionID";
			if(!mysqli_query($this->mysqlConnect,$query)) return "Не удалось удалить вопросы";
		}

		$query = "DELETE FROM questions WHERE id_theme=$themeID";
		if(!mysqli_query($this->mysqlConnect,$query)) return "Не удалось удалить вопросы";
		else return "Тема удалена успешно";
	}

	function CreateTest($name, $union, $year, $theme, $info, $testQuestionsNum, $numOfFullQuestions) {
		$name = addslashes($name);
		$union = addslashes($union);
		$theme = addslashes($theme);
		$info = addslashes($info);

		if(trim($name) =='' || trim($info) =='' || trim($testQuestionsNum) =='' || trim($numOfFullQuestions) =='') {
			return "не заполнены все поля";
		}
		$themeID = $this->GetThemeId($theme, $union, $year);
		if(!$themeID) return "Тема не найдена";

		$query = "INSERT INTO tests(id,num_of_test_questions,name,id_theme,num_of_full_questions,info)
				VALUES ('', $testQuestionsNum, \"$name\", $themeID, $numOfFullQuestions, \"$info\")";
		if(!mysqli_query($this->mysqlConnect,$query)) return "Тест не добавлен. $query";
		return 0;			
	}
}
?>