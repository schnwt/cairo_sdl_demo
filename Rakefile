require 'rake'
require 'fileutils'

directory 'out' do |t|
  puts 'Creating output directory...'
  Dir.mkdir(t.name) unless Dir.exists?(t.name)
end

rule(/\.o$/ => [
  proc {|tn| "src/#{File.basename(tn).ext('.c')}" }
]) do |t|
  sh "cc -c #{t.source} -o #{t.name}"
end

SRC = Dir['src/*.c'].map do |f|
  "out/#{File.basename(f).ext('.o')}"
end
DEPS = ['out'] + SRC
file 'out/soundmaker' => DEPS do |t|
  sh "cc #{SRC.join(' ')} -o #{t.name}"
end

task :run => 'out/soundmaker' do |t|
  sh t.prerequisites.first
end

task :clean do
  FileUtils.rm_rf('out')
end

task :default => :run
