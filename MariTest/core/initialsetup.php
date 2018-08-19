<? //Этот файл первоначальной настройки сайта. Загружается список прав в БД, а также создаётся первый юзер "admin"
require_once("db.php");
echo '<meta http-equiv="Content-Type" content="text/html; charset=UTF-8"/>';
$dbControl = new DB();

//Создания списка прав.
$rights = array(
	//имя права 		=> 	информация о нём
	'create test' 		=> 'Разрешает создавать тесты',
	'create question' 	=> 'Разрешает создавать вопросы',
	'create union'		=>  'Разрешает создавать объединения',
	'approve user'		=> 'Разрешает верифицировать пользователей, присоединивщихся к объединениям',
	'create theme'		=> 'Позволяет создавать темы'
);

foreach ($rights as $right => $info) {
	if($dbControl->CreateRight($right, $info)) {
		echo "Право $right already exists<br/>";
	}
}

//Создание списка объединений по-умолчанию.
$unions = array(
	// имя объединения => информация о нём.
	'test_union' => 'Здесь надо писать инфу' 
);

foreach ($unions as $name => $info) {
	if($dbControl->CreateUnion($name,$info)) {
		echo "Объединение $name already exists<br/>";
	}
}

//Добавление первого юзера-админа.
$user = array(
	'login'				=> "Mari",
	'password'			=> "admin",
	'passwordRepeat'	=> "admin",
	'name'				=> "Мария",
	'middleName'		=> "Юрьевна",
	'lastName'			=> "Румянцева",
	'code'				=> $dbControl->teacherCode,
	'unions'			=> array()
);

echo  $dbControl->RegisterUser($user), "<br/>";
?>