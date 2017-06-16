from elections.models import *
from elections import utility
from rest_framework import serializers


class CandidateSerializer(serializers.ModelSerializer):

    url = serializers.HyperlinkedIdentityField(view_name='elections:candidate-detail',
                                               read_only=True)

    class Meta:
        model = Candidate
        fields = ['url', 'name']


class VoteSerializer(serializers.ModelSerializer):

    url = serializers.HyperlinkedIdentityField(view_name='elections:vote-detail',
                                               read_only=True)

    candidate = serializers.StringRelatedField()
    gmina = serializers.StringRelatedField()

    class Meta:
        model = Vote
        fields = ['url', 'gmina', 'candidate', 'votes']


class GminaStatsSerializer(serializers.Serializer):
    voters = serializers.IntegerField()
    ballots = serializers.IntegerField()
    votes_dict = serializers.DictField(child=serializers.IntegerField())

    def update(self, instance, validated_data):
        instance.voters = validated_data.get('voters', instance.voters)
        instance.ballots = validated_data.get('ballots', instance.ballots)
        votes_dict = self.validated_data.get('votes_dict', {})
        utility.updateVotes(instance, votes_dict)
        instance.save()
        utility.notifyWebSockets(instance)
        return instance


class GminaSerializer(serializers.ModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='elections:gmina-detail',
                                               read_only=True)

    powiat = serializers.HyperlinkedRelatedField(view_name='elections:powiat-detail',
                                                 read_only=True)

    voters = serializers.FloatField(source='getVoters')
    ballots = serializers.FloatField(source='getBallots')
    turnout = serializers.FloatField(source='getTurnout')
    distribution = serializers.ListField(
        child=serializers.DictField(child=serializers.StringRelatedField()), source='getDistribution')

    class Meta:
        model = Gmina
        fields = ['url', 'path', 'name', 'code',
                  'powiat', 'voters', 'ballots', 'turnout', 'distribution']


class GminaVoteStatsSerializer(serializers.ModelSerializer):
    votes = serializers.DictField(child=serializers.IntegerField())

    class Meta:
        model = Gmina
        fields = ['voters', 'ballots', 'votes']


class PowiatSerializer(serializers.HyperlinkedModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='elections:powiat-detail',
                                               read_only=True)

    wojewodztwo = serializers.HyperlinkedRelatedField(
        view_name='elections:wojewodztwo-detail', read_only=True)

    gmina_set = serializers.HyperlinkedRelatedField(
        many=True, view_name='elections:gmina-detail', read_only=True)

    voters = serializers.FloatField(source='getVoters')
    ballots = serializers.FloatField(source='getBallots')
    turnout = serializers.FloatField(source='getTurnout')
    distribution = serializers.ListField(
        child=serializers.DictField(child=serializers.StringRelatedField()), source='getDistribution')

    class Meta:
        model = Powiat
        fields = ['url', 'path' ,'name', 'wojewodztwo', 'gmina_set', 'voters',
                  'ballots', 'turnout', 'distribution']


class WojewodztwoSerializer(serializers.HyperlinkedModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='elections:wojewodztwo-detail',
                                               read_only=True)

    powiat_set = serializers.HyperlinkedRelatedField(
        many=True, view_name='elections:powiat-detail', read_only=True)

    voters = serializers.FloatField(source='getVoters')
    ballots = serializers.FloatField(source='getBallots')
    turnout = serializers.FloatField(source='getTurnout')
    distribution = serializers.ListField(
        child=serializers.DictField(child=serializers.StringRelatedField()), source='getDistribution')

    class Meta:
        model = Wojewodztwo
        fields = ['url', 'path', 'name', 'powiat_set', 'voters',
                  'ballots', 'turnout', 'distribution']


class KrajSerializer(serializers.HyperlinkedModelSerializer):
    url = serializers.HyperlinkedIdentityField(view_name='elections:kraj-detail',
                                               read_only=True)

    voters = serializers.FloatField(source='getVoters')
    ballots = serializers.FloatField(source='getBallots')
    turnout = serializers.FloatField(source='getTurnout')
    distribution = serializers.ListField(
        child=serializers.DictField(child=serializers.StringRelatedField()), source='getDistribution')

    class Meta:
        model = Wojewodztwo
        fields = ['url', 'path', 'name', 'voters',
                  'ballots', 'turnout', 'distribution']
