<div class="login">
	<h2 class="login"><span>Login</span></h2>
	<p class="required-info">Please enter your Username and e-mail address then click on the Send Password button.<br />You will receive a new password shortly. Use this new password to access the site.</p>
	<form action="<?=base_path()?>user/"  method="post" id="user-login">
		<div class="row">
			<label for="name"><strong>Username</strong></label>
			<input type="text" id="username" name="name" class="text"/>
		</div>
		<div class="row">
			<label for="pass"><strong>Password</strong></label>
			<input type="password" id="pass" name="pass" class="text"/>
		</div>
		<div class="row-submit">
			<input type="hidden" name="form_id" id="edit-user-login" value="user_login"  />
			<input type="image" src="<?=base_path().path_to_theme()?>/images/grid/button_login.gif" alt="Login" title="Login" onmouseover="this.src = '<?=base_path().path_to_theme()?>/images/grid/button_login_on.gif';" onfocus="this.src = '<?=base_path().path_to_theme()?>/images/grid/button_login_on.gif';" onblur="this.src = '<?=base_path().path_to_theme()?>/images/grid/button_login.gif';" onmouseout="this.src = '<?=base_path().path_to_theme()?>/images/grid/button_login.gif';"/>
		</div>
	</form>
	<p><a href="user/password">Lost password?</a></p>
</div>