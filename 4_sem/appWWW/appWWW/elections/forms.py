from django.forms import *


class EditGminaForm(Form):

    def __init__(self, *args, **kwargs):
        data = kwargs.pop('data', [])
        ballots = kwargs.pop('ballots', 0)
        voters = kwargs.pop('voters', 0)
        super(EditGminaForm, self).__init__(*args, **kwargs)

        self.fields["ballots"] = IntegerField()
        self.fields["ballots"].initial = ballots
        self.fields["ballots"].label = "Kart ważnych"

        self.fields["voters"] = IntegerField()
        self.fields["voters"].initial = voters
        self.fields["voters"].label = "Uprawnionych"

        for candidate_data in data:
            candidate, votes = candidate_data
            name = candidate.name
            self.fields[name + "_votes"] = IntegerField()
            self.fields[name + "_votes"].initial = votes
            self.fields[name + "_votes"].label = name

    def clean(self):
        cleaned_data = super(EditGminaForm, self).clean()
        ballots = cleaned_data.get('ballots')
        voters = cleaned_data.get('voters')
        if ballots > voters:
            raise ValidationError("Liczba upoważnionych jest mniejsza od liczby kart ważnych", code="ballots_voters_error")
        return cleaned_data

class SearchForm(Form):
    query = CharField(required=True, label='Wyszukuj gminę')
