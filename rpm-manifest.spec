Name:           matrix
Version:        1.0.0
Release:        1%{?dist}
Summary:        A simple matrix library for C++
BuildArch:      noarch

License:        MIT
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc-c++

%description
A RPM package for TSPP

%prep
%setup -q

%install
mkdir -p %{buildroot}${INSTALL_DIR}
install -m 644 src/matrix.hpp %{buildroot}${INSTALL_DIR}/matrix.hpp

%clean
rm -rf $RPM_BUILD_ROOT

%files
${INSTALL_DIR}/matrix.hpp

%changelog
* ${CHANGELOG_DATE} Your Name <your.email@example.com> - ${VERSION}-${RELEASE}
- Initial package