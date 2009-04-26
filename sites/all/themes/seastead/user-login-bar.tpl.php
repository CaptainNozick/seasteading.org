<div id="block-user-0" class="clear-block block block-user">
	<div class="content">
		<?php  global $user; ?>
		<?php if ($user->uid) : ?>
			<div class="logged-in">
				<?php print(t('Logged in as:'))?> <?php print l($user->name,'user/'.$user->uid); ?>
				 | <?php print l((t('log out')),"logout"); ?>
			</div>
		<?php else : ?>
			<form action="<?=base_path()?>user/login/?<?=drupal_get_destination()?>" method="post" id="user-login-form" accept-charset="UTF-8">
				<div>
					<!--<input type="hidden" name="edit[destination]" value="user" />-->
					<a href="<?php print base_path() ?>user/register" title="Register" class="register"><span>Register</span></a>
					<span class="or">or</span>
					<div class="form-item">
						<!--<input type="text" maxlength="60" name="edit[name]" id="edit-name" size="15" value="Username:" class="form-text required" />-->
						<input type="text" maxlength="60" name="name" id="edit-name" size="15" value="" class="form-text required" />
					</div>
					<div class="form-item">
						<!--<input type="password" name="edit[pass]" id="edit-pass" maxlength="60" size="15" class="form-text required" />-->
						<input type="password" name="pass" id="edit-pass" maxlength="60" size="15" class="form-text required" />
					</div>
					<input type="image" src="<?php print base_path() . path_to_theme() ?>/images/button_next.gif" name="op" id="edit-submit" value="Log in" class="form-submit" />
					<p class="forgot"><a href="<?php print base_path() ?>user/password" title="Forgot password?">Forgot password?</a></p>
					<!--<input type="hidden" name="edit[form_id]" id="edit-form_id" value="user_login"  />-->
					<input type="hidden" name="form_id" id="edit-user-login" value="user_login"  />
				</div>
			</form>
		<?php endif; ?>
		<?php if ($search_box): ?><div class="block block-theme"><?php print $search_box ?></div><?php endif; ?>
	</div>
</div>
