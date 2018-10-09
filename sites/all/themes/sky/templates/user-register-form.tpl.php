
<div id="user-register-page">
	<table class="user-register-general">
		<tr class="user-register-general">
			<td class="user-register-general">
				<table class="user-register-first">
					<td class="user-register-first">
						<?php/*
							if(isset($form['account']['name']))
							{
								print render($form['account']['name']);
							}
							*/
						?>
					</td>
				</table>
			</td>
		</tr>
	</table>
</div>


<?php 
  print render($form['form_id']);
  print render($form['form_build_id']);
  print render($form['account']['name']);
  print render($form['account']['pass']);
  print render($form['account']['mail']);
  print render($form['field_achternaam']);
  print render($form['field_voornaam']);
  print render($form['field_gsmnr']);
  print render($form['field_email']);
  print render($form['field_street']);
  print render($form['field_hnummer']);
  print render($form['field_busnr']);
  print render($form['field_postcode']);
  print render($form['field_gemeente']);
  print render($form['captcha']);
  print drupal_render($form['actions']); 
  print drupal_render_children($form);
  
?>
