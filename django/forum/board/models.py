from django.db import models
from django.contrib.auth.models import User
from django.core.exceptions import ValidationError
import datetime


class Category(models.Model):
    name = models.CharField(max_length = 150)
    
    last_topic = models.ForeignKey('Topic', null = True, default = None, editable = False, related_name = "last_topic_in_category")
    topics = models.PositiveIntegerField(editable = False, default = 0)
    posts = models.PositiveIntegerField(editable = False, default = 0)
    
    def __unicode__(self):
        return self.name
    
class Topic(models.Model):
    name = models.CharField(max_length = 200)
    desc = models.CharField(max_length = 1000, blank = True)
    category = models.ForeignKey('Category')
    date_created = models.DateTimeField(auto_now_add = True)
    user = models.ForeignKey(User)
    
    last_post = models.DateTimeField(auto_now_add = True)
    posts = models.PositiveIntegerField(editable = False, default = 0)
    visits = models.PositiveIntegerField(editable = False, default = 0)
    
    def __unicode__(self):
        return self.name
    
    def increase_visits(self):
        self.visits += 1
        self.save()
    
    def save(self, *args, **kwargs):
        if not self.id:
            self.category.topics += 1
            self.category.save()
        super(Topic, self).save(*args, **kwargs)
    
class Post(models.Model):
    topic = models.ForeignKey(Topic)
    date_created = models.DateTimeField(auto_now_add = True)
    message = models.TextField()
    user = models.ForeignKey(User)
    
    def save(self, *args, **kwargs):
        if not self.id:
            self.topic.posts += 1
            self.topic.category.posts += 1
            self.topic.last_post = datetime.datetime.now()
            self.topic.save()
        self.topic.category.last_topic = self.topic
        self.topic.category.save()
        super(Post, self).save(*args, **kwargs)
    
class Settings(models.Model):
    posts_per_page = models.PositiveIntegerField(default = 20)
    topics_per_page = models.PositiveIntegerField(default = 15)
    
    def save(self, *args, **kwargs):
        if self.id:
            super(Settings, self).save(*args, **kwargs)
        
    def delete(self):
        pass
