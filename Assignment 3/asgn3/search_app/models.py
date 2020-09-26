from django.db import models

# Create your models here.
class QueryModel(models.Model):
    keywords = models.TextField()
    image = models.ImageField(upload_to = "images/")

    def __str__(self):
        return self.keywords