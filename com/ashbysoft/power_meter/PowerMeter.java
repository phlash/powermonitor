// GUI display of waveform from power metering head
package com.ashbysoft.power_meter;

import java.io.File;
import java.io.InputStreamReader;
import java.io.BufferedReader;
import java.net.URLDecoder;

import java.awt.Graphics;
import java.awt.Component;
import java.awt.Color;

import javax.swing.SwingUtilities;
import javax.swing.JFrame;

public class PowerMeter extends Component implements Runnable {
	private File serio;
	private int spos = 0;
	private short samples[] = new short[1000];

	public static void main(String[] args) throws Exception {
		PowerMeter me = new PowerMeter(args);
		SwingUtilities.invokeLater(me);
	}

	PowerMeter(String[] args) throws Exception {
		// Check serio interface program is available
		String p1 = getClass().getProtectionDomain().getCodeSource().getLocation().getPath();
		String base = URLDecoder.decode(p1, "UTF-8");
		if (base.endsWith(".jar")) {
			base = base.substring(0, base.lastIndexOf('/'));
		}
		this.serio = new File(base + File.separator + "serio");
		if (!serio.exists()) {
			throw new Exception("missing serio program in jar folder");
		}
	}

	public void paint(Graphics g) {
		g.setColor(Color.BLACK);
		g.fillRect(0,0,getWidth(), getHeight());
		g.setColor(Color.GREEN);
		int ly = getHeight()/2;
		int oy = ly;
		short mn = 32767, mx = -32767;
		long ms = 0;
		for (int i=1; i<getWidth(); i++) {
			int y = i<samples.length ? oy+(int)samples[i] : oy;
			g.drawLine(i-1,ly,i,y);
			ly = y;
			if (i<samples.length) {
				mn = (samples[i]<mn) ? samples[i] : mn;
				mx = (samples[i]>mx) ? samples[i] : mx;
				ms += (long)samples[i]*(long)samples[i];
			}
		}
		ms = (long)Math.sqrt(ms/samples.length);
		g.drawString("min:"+mn+" max:"+mx+" rms:"+ms, 10, 10);
	}

	public void run() {
		JFrame frame = new JFrame("Power Meter");
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setSize(samples.length, 600);
		frame.add(this);
		frame.setVisible(true);
		new Thread(new Runnable() {
			public void run() {
				try {
					// Kick off serio, plot waveform scope style..
					Process pserio = Runtime.getRuntime().exec(serio.toString());
					BufferedReader bread = new BufferedReader(new InputStreamReader(pserio.getInputStream()));
					String line;
					while ((line=bread.readLine())!=null) {
						if (line.length()>0) {
							try {
								int t = Integer.parseInt(line, 16);
								samples[spos] = (short)t;
							} catch (NumberFormatException nf) {
								System.out.println("unparsable="+line);
							}
							if (++spos>=samples.length) {
								spos=0;
							}
							if (spos%100==0)
								repaint();
						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();
	}
}
