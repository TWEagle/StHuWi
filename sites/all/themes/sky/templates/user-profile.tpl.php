<div id="user-profile">
	<table class="user-info">
		<tr class="user-info">
			<td class="user-info">
				<table class="users">
					<td class="user-info">
						<?php 
							if(isset($user_profile['user_picture']['#markup']))
							{
								print $user_profile['user_picture']['#markup'];
							}
						?>
					</td>
					<td class="user-info">
						<b>
							<font size='4px'>
								<?php 
									if(isset($user_profile['field_achternaam'][0]['#markup']) || isset($user_profile['field_voornaam'][0]['#markup']))
									{
										print 
										$user_profile['field_achternaam'][0]['#markup'] .' '.
										$user_profile['field_voornaam'][0]['#markup'];
									}
								?>
							</font>
						</b>
					</td>
					<td class="user-info">
						<b>GSM nummer: </b>
					</td>
					<td class="user-info">
						<?php
							if(isset($user_profile['field_gsmnr'][0]['#markup']))
							{
								print $user_profile['field_gsmnr'][0]['#markup'];
							}
						?>
					</td>
					<td class="user-info">
						<b>E-mailadres: </b>
					</td>
					<td class="user-info">
						<?php
							if(isset($user_profile['field_email'][0]['#markup']))
							{
								print $user_profile['field_email'][0]['#markup'];
							}
						?>
					</td>
				</table>
			</td>
		</tr>
	</table>
	<table class="user-adres">
		<tr class="user-adres">
			<td class="user-adres">
				<table class="users-adres">
					<td class="user-adres">
						<font size='4px'>
							<?php
								if(isset($user_profile['field_street'][0]['#markup']) || isset($user_profile['field_hnummer'][0]['#markup']) || isset($user_profile['field_bnummer'][0]['#markup']) || isset($user_profile['field_postcode'][0]['#markup']) || isset($user_profile['field_gemeente'][0]['#markup']))
								{
									print $user_profile['field_street'][0]['#markup'] .' '. $user_profile['field_hnummer'][0]['#markup'] .' '. $user_profile['field_bnummer'][0]['#markup'] .' '. $user_profile['field_postcode'][0]['#markup'] .' '. $user_profile['field_gemeente'][0]['#markup'];
								}
							?>
						</font>
					</td>
				</table>
			</td>
		</tr>
	</table>
		<?php if (isset($user_profile['user_picture']['#markup'])): ?>

		<?php endif; ?>
</div>
