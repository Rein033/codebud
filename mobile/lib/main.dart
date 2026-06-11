import 'package:flutter/material.dart';

import 'codebud_ble.dart';

void main() {
  runApp(const CodeBudApp());
}

class CodeBudApp extends StatelessWidget {
  const CodeBudApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'CodeBud',
      theme: ThemeData(colorSchemeSeed: Colors.blue, useMaterial3: true),
      home: const HomePage(),
    );
  }
}

const _apps = ['Clock', 'Pomodoro', 'Pong', 'Sensor', 'Messages'];

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  final _ble = CodeBudBle();
  final _titleController = TextEditingController();
  final _messageController = TextEditingController();

  bool _connecting = false;
  bool _connected = false;
  String _status = '-';

  @override
  void initState() {
    super.initState();
    _ble.connectionState.listen((connected) {
      if (!mounted) return;
      setState(() => _connected = connected);
    });
    _ble.status.listen((status) {
      if (!mounted) return;
      setState(() => _status = status);
    });
  }

  @override
  void dispose() {
    _ble.disconnect();
    _titleController.dispose();
    _messageController.dispose();
    super.dispose();
  }

  Future<void> _toggleConnection() async {
    if (_connected) {
      await _ble.disconnect();
      return;
    }

    setState(() => _connecting = true);
    final ok = await _ble.scanAndConnect();
    setState(() => _connecting = false);

    if (!ok && mounted) {
      ScaffoldMessenger.of(context).showSnackBar(
        const SnackBar(content: Text('Kon geen CodeBud device vinden')),
      );
    }
  }

  Future<void> _openApp(String name) async {
    try {
      await _ble.openApp(name);
    } catch (e) {
      _showError(e);
    }
  }

  Future<void> _sendNotification() async {
    try {
      await _ble.sendNotification(_titleController.text, _messageController.text);
      if (mounted) {
        ScaffoldMessenger.of(context).showSnackBar(
          const SnackBar(content: Text('Bericht verstuurd')),
        );
      }
    } catch (e) {
      _showError(e);
    }
  }

  void _showError(Object e) {
    if (!mounted) return;
    ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text('Fout: $e')));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: const Text('CodeBud')),
      body: ListView(
        padding: const EdgeInsets.all(16),
        children: [
          Card(
            child: ListTile(
              leading: Icon(
                _connected ? Icons.bluetooth_connected : Icons.bluetooth_disabled,
                color: _connected ? Colors.blue : Colors.grey,
              ),
              title: Text(_connected ? 'Verbonden' : 'Niet verbonden'),
              subtitle: Text('Actief scherm: $_status'),
              trailing: FilledButton(
                onPressed: _connecting ? null : _toggleConnection,
                child: Text(
                  _connecting ? '...' : (_connected ? 'Verbreken' : 'Verbinden'),
                ),
              ),
            ),
          ),
          const SizedBox(height: 16),
          Text('Apps openen', style: Theme.of(context).textTheme.titleMedium),
          const SizedBox(height: 8),
          Wrap(
            spacing: 8,
            runSpacing: 8,
            children: [
              for (final app in _apps)
                ActionChip(
                  label: Text(app),
                  onPressed: _connected ? () => _openApp(app) : null,
                ),
            ],
          ),
          const SizedBox(height: 24),
          Text('Bericht sturen', style: Theme.of(context).textTheme.titleMedium),
          const SizedBox(height: 8),
          TextField(
            controller: _titleController,
            decoration: const InputDecoration(labelText: 'Titel', border: OutlineInputBorder()),
          ),
          const SizedBox(height: 8),
          TextField(
            controller: _messageController,
            decoration: const InputDecoration(labelText: 'Bericht', border: OutlineInputBorder()),
            maxLines: 3,
          ),
          const SizedBox(height: 8),
          FilledButton(
            onPressed: _connected ? _sendNotification : null,
            child: const Text('Verstuur naar CodeBud'),
          ),
        ],
      ),
    );
  }
}
