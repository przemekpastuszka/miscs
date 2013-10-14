from board.models import Category, Topic, Post, Settings
from django.contrib import admin

#~ class CategoryAdmin(admin.ModelAdmin):
    #~ list_display = ("__unicode__", "name", "topics_count")
    #~ list_display_links = ("__unicode__", )
    #~ list_editable = ("name",)
    #~ ordering = ("name", )

admin.site.register(Category)
admin.site.register(Topic)
admin.site.register(Post)
admin.site.register(Settings)
