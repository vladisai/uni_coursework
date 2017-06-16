from django.test import LiveServerTestCase
from django.contrib.auth.models import User
from django.db import transaction

from time import sleep

import populate

from elections import models

from selenium import webdriver


class TheTestCase(LiveServerTestCase):

    def setUp(self):
        User.objects.create_superuser(
            username='admin',
            password='admin',
            email='admin@admin.com')
        self.selenium = webdriver.Chrome()
        self.selenium.maximize_window()

        with transaction.atomic():
            kraj = models.Kraj.objects.create(name='Polska')
            kraj.save()
            wojewodztwo = models.Wojewodztwo.objects.create(
                name='Brześć', kraj=kraj)
            powiat = models.Powiat.objects.create(
                name='Brześć', wojewodztwo=wojewodztwo)
            gmina = models.Gmina.objects.create(
                name='Brześć', code=123, powiat=powiat)

            candidate = models.Candidate.objects.create(name='Lukashenko')
            vote = populate.constructVote(
                wojewodztwo, powiat, gmina, candidate, 15)
            candidate.save()
            vote.save()

            gmina.voters += 30
            gmina.ballots += 15

            kraj.save()
            wojewodztwo.save()
            powiat.save()
            gmina.save()
            candidate.save()
            vote.save()

        super(TheTestCase, self).setUp()

    def tearDown(self):
        self.selenium.quit()
        super(TheTestCase, self).tearDown()

    def test_test(self):
        self.selenium.get(self.live_server_url)
        self.selenium.implicitly_wait(20)
        self.selenium.find_element_by_id('login_link').click()
        self.selenium.find_element_by_name('uname').send_keys('admin')
        self.selenium.find_element_by_name('psw').send_keys('admin')
        self.selenium.find_element_by_id('submit_button').click()
        self.selenium.find_element_by_xpath(
            "//a[starts-with(text(), 'Brześć')]").click()
        self.selenium.find_element_by_xpath(
            "//a[starts-with(text(), 'Brześć')]").click()
        self.selenium.find_element_by_xpath(
            "//a[starts-with(text(), 'Brześć')]").click()
        self.selenium.find_element_by_xpath(
            "//a[starts-with(text(), 'Edytuj')]").click()
        self.selenium.find_element_by_id('voters').clear()
        self.selenium.find_element_by_id('voters').send_keys('100')
        self.selenium.find_element_by_id('ballots').clear()
        self.selenium.find_element_by_id('ballots').send_keys('100')
        self.selenium.find_element_by_id('Lukashenko').clear()
        self.selenium.find_element_by_id('Lukashenko').send_keys('100')
        self.selenium.find_element_by_xpath("//input[@type='submit']").click()
        self.selenium.back()
        self.selenium.refresh()

        sleep(1) # bad style, but WebDriverWait was useless, like many other things I tried

        voters = self.selenium.find_element_by_xpath(
            "//span[string-length(text()) > 1][@id='header_voters']")
        assert voters.text == '100'

        ballots = self.selenium.find_element_by_xpath(
            "//span[string-length(text()) > 1][@id='header_ballots']")
        assert ballots.text == '100'
