from django import forms

class NewTopicForm(forms.Form):
    topic_name = forms.CharField(max_length = 200)
    topic_desc = forms.CharField(max_length = 1000, required = False)
    post_message = forms.CharField(widget = forms.Textarea)

class NewPostForm(forms.Form):
    message = forms.CharField(widget = forms.Textarea)
