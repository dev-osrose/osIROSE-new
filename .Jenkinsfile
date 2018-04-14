pipeline {
  agent {
    docker { image 'ravenx8/ubuntu-dev' }
  }
  stages {
    stage('Preparation') {
      steps {
        checkout scm
        dir('osIROSE-new') {
          sh 'git submodule update --init --recursive'
          sh 'mkdir build'
        }
      }
    }
    stage('Build on Linux') { 
      agent {
        label 'linux'
      }
      steps {
        dir('osIROSE-new/build') {
          sh 'cmake --build .'
        }
      }
    }
    //stage('Build on Windows') { 
    //  agent {
    //    label 'windows'
    //  }
    //  steps {
    //    dir('osIROSE-new/build') {
    //      bat 'cmake --build .'
    //    }
    //  }
    //}
    stage('Test') { 
      steps {
        sh 'ctest .'
      }
    }
  }
  
  post {
    always {
      cleanWs notFailBuild: true
    }
    
    failure {
      echo 'Build Failed'
    }
  }
}