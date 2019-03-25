from pyramid.config import Configurator
from sqlalchemy import engine_from_config
from services.models import DBSession, Base

Config = None


def main(global_config, **settings):
    engine = engine_from_config(settings, 'sqlalchemy.')
    DBSession.configure(bind=engine)
    Base.metadata.bind = engine

    global Config
    Config = Configurator(
        settings=settings, root_factory='services.models.Root')

    Config.include('pyramid_chameleon')
    Config.add_static_view('deform_static', 'deform:static/')

    Config.scan('.stats_views')

    return Config.make_wsgi_app()
