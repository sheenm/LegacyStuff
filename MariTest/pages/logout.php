<? //Заканчивает сеанс пользователя
session_unset();
session_destroy();

header('Location:?page=login');

?>