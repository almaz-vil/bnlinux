const Applet = imports.ui.applet;
const Settings = imports.ui.settings;
const Lang = imports.lang;
const Gio = imports.gi.Gio;
const GLibFile = imports.gi.GLib.IOChannel;

function MyApplet(orientation, panelHeight, instanceId) {
  this._init(orientation, panelHeight, instanceId);
}

MyApplet.prototype = {
  __proto__: Applet.TextApplet.prototype,
  _init: function(orientation, panelHeight, instanceId) {
      Applet.TextApplet.prototype._init.call(this, orientation, panelHeight, instanceId);  
      try {
        this.settings = new Settings.AppletSettings(this, "bnlinux@bnabor.ru", this.instanceId);
        this.settings.bind("font-size", "font", this._onTrashChange);
        this.settings.bind("color", "color", this._onTrashChange);
        this.settings.bind("background-color", "bg_color", this._onTrashChange);
        this.trash_path = 'file:///tmp/bnlinux_sv';
        this.trash_file =  Gio.file_new_for_uri(this.trash_path);
        this.trash_changed_timeout = 0;  
        this.monitor = this.trash_file.monitor_file(0, null);
        this.monitor.connect('changed', Lang.bind(this, this._onTrashChange));
      }
      catch (e) {
          global.logError(e);
      }
    },
    _onTrashChange() {
      this._applet_label.set_style('min-width: 10.5em; color:'+this.color+';  background-color:'+this.bg_color+'; font-size: '+this.font+'px; text-align: left');
      this.idle=GLibFile.new_file("/tmp/bnlinux_sv", "r");
      this.re=this.idle.read_line();

      this.set_applet_label(this.re[1]);
    },
};

function main(metadata, orientation, panelHeight, instanceId) {
   let myApplet = new MyApplet(orientation, panelHeight, instanceId);
   return myApplet
}
