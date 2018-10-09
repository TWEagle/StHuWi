/**
 * @file
 *  Two steps login behaviours.
 */
var Two_steps_login = Two_steps_login || {};
Drupal.settings = Drupal.settings || {};

(function ($) {

  'use strict';

  Drupal.behaviors.two_steps_login_form = {
    attach: function (context) {
      Two_steps_login.two_steps_login_form.initialize(context);
    }
  };

  Two_steps_login.two_steps_login_form = {

    HIDDEN_CLASS: 'two-steps-login-hidden',
    TWO_STEPS_LOGIN_COOKIE: 'two_steps_login_usernames',

    initialize: function () {
      this.bindEvents();
      if (this.getUserNameInputFieldOnForm().hasClass('error')) {
        this.getNotYouElement().trigger('click');
      }
      else if (this.getUserNameWrapper().length) {
        this.hideUserNameField();
        this.hideNextButton();
      }
      else {
        this.showUserNameField();
      }
    },

    getUserNameWrapper: function () {
      return $('#usernames-wrapper');
    },

    hideUserNameField: function () {
      this.getUserInputFieldWrapper().hide();
      this.getUserNameFieldOnForm().hide();
    },

    hideNextButton: function () {
      this.getTwoStepLoginFormSubmitButton().hide();
    },

    showNextButton: function () {
      this.getTwoStepLoginFormSubmitButton().show();
    },

    showUserNameField: function () {
      this.getUserNameFieldOnForm().show();
      this.getUserInputFieldWrapper().show();
    },

    showUserNameDefaultPic: function () {
      this.getUserNameDefaultPic().removeClass(this.HIDDEN_CLASS);
    },

    hideUserNameDefaultPic: function () {
      this.getUserNameDefaultPic().addClass(this.HIDDEN_CLASS);
    },

    getUserNameFieldOnForm: function () {
      return $('.two-steps-login-ajax-form .form-item-name');
    },

    getUserNameInputFieldOnForm: function () {
      return $('.two-steps-login-ajax-form .form-item-name input.form-text');
    },

    getUserInputFieldWrapper: function () {
      return $('.two-steps-login-username-input-wrapper');
    },

    getUserNameField: function () {
      return $('#edit-name');
    },

    getUserNameDefaultPic: function () {
      return $('#user-default-pic');
    },

    getUserNameTextDiv: function () {
      return $('.two-steps-login-user-names');
    },

    getUserNameOuterDivWrapper: function () {
      return $('.two-steps-login-container');
    },

    getUserNameContainer: function () {
      return $('.two-steps-login-names-wrapper');
    },

    getNotYouElement: function () {
      return $('#two-steps-login-not-you');
    },

    getManageAccountElement: function () {
      return $('#two-steps-login-manage-account');
    },

    getShowMoreDiv: function () {
      return $('.show-more-user-names');
    },

    getNextButton: function () {
      return $('.show-more-user-names');
    },

    getTwoStepLoginFormSubmitButton: function () {
      return $('.two-steps-login-ajax-form form .form-submit');
    },

    getTwoStepsLoginFooter: function () {
      return $('#two-steps-footer');
    },

    getTwoStepsLoginRegisterLinks: function () {
      return $('#two-steps-footer-links');
    },

    getRemoveElement: function () {
      return $('.two-steps-login-remove');
    },

    getForwardElement: function () {
      return $('.two-steps-login-forward');
    },

    getfinishManageAccountElement: function () {
      return $('#two-steps-login-finish-manage-account');
    },

    bindEvents: function () {
      this.getUserNameContainer().bind('click', $.proxy(function (e) {
        var username = $(e.currentTarget).find('div.two-steps-login-user-names').attr('user-name');
        if (Drupal.settings.two_steps_login.is_email_registration_enabled) {
          username = $(e.currentTarget).find('div.two-steps-login-user-names').attr('user-email');
        }
        this.getUserNameField().val(username);
        this.getTwoStepLoginFormSubmitButton().trigger('mousedown');
      }, this));

      this.getNotYouElement().bind('click', $.proxy(function () {
        this.getUserNameWrapper().hide();
        this.getTwoStepsLoginFooter().hide();
        this.getTwoStepsLoginRegisterLinks().hide();
        this.getUserNameField().val('');
        this.showUserNameField();
        this.showUserNameDefaultPic();
        this.showNextButton();
      }, this));

      this.getManageAccountElement().bind('click', $.proxy(function () {
        var that = this;
        this.getfinishManageAccountElement().removeClass(this.HIDDEN_CLASS);
        this.getNotYouElement().hide();
        this.getManageAccountElement().hide();
        this.getUserNameOuterDivWrapper().each(function () {
          $(this).removeClass(that.HIDDEN_CLASS);
        });
        this.getShowMoreDiv().hide();
        this.getRemoveElement().each(function () {
          $(this).removeClass(that.HIDDEN_CLASS);
        });
        this.getForwardElement().each(function () {
          $(this).addClass(that.HIDDEN_CLASS);
        });
      }, this));

      this.getfinishManageAccountElement().bind('click', $.proxy(function () {
        var that = this;
        this.getfinishManageAccountElement().addClass(this.HIDDEN_CLASS);
        this.getNotYouElement().show();
        this.getManageAccountElement().show();
        this.getRemoveElement().each(function () {
          $(this).addClass(that.HIDDEN_CLASS);
        });
        this.getForwardElement().each(function () {
          $(this).removeClass(that.HIDDEN_CLASS);
        });
      }, this));

      this.getShowMoreDiv().bind('click', $.proxy(function () {
        var that = this;
        this.getUserNameOuterDivWrapper().each(function () {
          $(this).removeClass(that.HIDDEN_CLASS);
        });
        this.getShowMoreDiv().hide();
      }, this));

      this.getRemoveElement().bind('click', $.proxy(function (e) {
        var id = $(e.currentTarget).attr('id');
        var uid = id.split('-').pop();
        var name = this.TWO_STEPS_LOGIN_COOKIE + '=';
        var decodedCookie = decodeURIComponent(document.cookie);
        var ca = decodedCookie.split(';');
        var jsonstring = '';
        for (var i = 0; i < ca.length; i++) {
          var c = ca[i];
          while (c.charAt(0) === ' ') {
            c = c.substring(1);
          }
          if (c.indexOf(name) === 0) {
            jsonstring = c.substring(name.length, c.length);
          }
        }
        var arrayusers = JSON.parse(jsonstring);
        delete arrayusers[uid];
        // adding updated json string to cookie.
        var newstring = JSON.stringify(arrayusers);
        var d = new Date();
        d.setTime(d.getTime() + (30 * 24 * 60 * 60 * 1000));
        var expires = 'expires=' + d.toUTCString();
        document.cookie = this.TWO_STEPS_LOGIN_COOKIE + '=' + newstring + ';' + expires + ';path=/';
        // cookie updated

        // hiding previous element
        $(e.currentTarget).parent().remove();
        $(e.currentTarget).remove();
        if (!this.getUserNameWrapper().find('.two-steps-login-container').length > 0) {
          this.getUserNameWrapper().hide();
          this.getTwoStepsLoginFooter().hide();
          this.getUserNameFieldOnForm().show();
          this.showNextButton();
        }
        if (this.getUserNameContainer().length === 0) {
          this.getNotYouElement().trigger('click');
          this.showUserNameDefaultPic();
        }
      }, this));
    }
  };

})(jQuery);
