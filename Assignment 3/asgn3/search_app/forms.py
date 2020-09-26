from django import forms

class QueryForm(forms.Form):
    keyword_input = forms.CharField(label='Keywords',required=True)
    num_neigh_input = forms.IntegerField(label='Number of neighbours',required=True, min_value=1)
    image_input = forms.ImageField(label='Sample image',required=True)

    def clean(self):
        super().clean()