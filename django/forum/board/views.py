from django.shortcuts import get_object_or_404, render, redirect
from django.core.paginator import Paginator, InvalidPage, EmptyPage
from board.models import Category, Topic, Settings, Post
from django.contrib.auth.decorators import login_required

def get_settings():
    return get_object_or_404(Settings, pk = 1)

def calculate_the_page(request, paginator):
    try:
        page = int(request.GET.get('page', '1'))
    except ValueError:
        page = 1
        
    try:
        result = paginator.page(page)
    except (EmptyPage, InvalidPage):
        result = paginator.page(paginator.num_pages)
    return result

def cat_index(request, cat_id):
    cat = get_object_or_404(Category, pk = cat_id)
    ordering = '-last_post'
    if request.GET.get('ordering', '-last_post') in ['-last_post', '-visits', '-posts']:
        ordering = request.GET.get('ordering', '-last_post')
    paginator = Paginator(cat.topic_set.order_by(ordering), get_settings().topics_per_page)
    
    return render(request, 'board/cat_index.html', 
    {'ordering' : ordering, 'category': cat, 'topics' : calculate_the_page(request, paginator)})
   
def topic(request, topic_id):
    t = get_object_or_404(Topic, pk = topic_id)
    paginator = Paginator(t.post_set.order_by('date_created'), get_settings().posts_per_page)
    t.increase_visits()
    
    return render(request, 'board/topic.html', {'topic': t, 'posts' : calculate_the_page(request, paginator)})

def index(request):
    return render(request, 'board/index.html', {'categories' : Category.objects.order_by('name')})

def users(request, user_id):
    return render(request, 'board/index.html', {'categories' : Category.objects.order_by('name')})

def user_input(request, Form, process, redirect_to, render_it):
    if request.method == 'POST':
        form = Form(request.POST)
        if form.is_valid():
            process(request, form.cleaned_data)
            return redirect_to()
    else:
        form = Form()
    return render_it(form)


from board.forms import NewTopicForm, NewPostForm

def new_topic_process(category, request, data):
    t = Topic(name = data['topic_name'], desc = data['topic_desc'],
        category = category, user = request.user)
    t.save()
    post = Post(topic = t, user = request.user, message = data['post_message'])
    post.save()

@login_required
def new_topic(request, cat_id):
    cat = get_object_or_404(Category, pk = cat_id)
    return user_input(request, NewTopicForm, 
        lambda x, y : new_topic_process(cat, x, y),
        lambda : redirect('board.views.cat_index', cat_id = cat_id),
        lambda f : render(request, 'board/new_topic.html', {'form': f, 'cat' : cat}))

def new_post_process(t, request, data):
    post = Post(topic = t, user = request.user, message = data['message'])
    post.save()

@login_required
def new_post(request, topic_id):
    topic = get_object_or_404(Topic, pk = topic_id)
    return user_input(request, NewPostForm, 
        lambda x, y : new_post_process(topic, x, y),
        lambda : redirect('board.views.topic', topic_id = topic_id),
        lambda f : render(request, 'board/new_post.html', {'form': f, 'topic' : topic}))
